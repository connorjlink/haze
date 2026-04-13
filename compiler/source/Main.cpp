import std;

#include <async/include/Task.h>

#include <allocator/HeapAllocator.h>
#include <allocator/RuntimeAllocator.h>
#include <allocator/StackAllocator.h>
#include <cli/CommandLineParser.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <io/FileManager.h>
#include <io/WebSocketServer.h>
#include <job/AutoJob.h>
#include <job/JobManager.h>
#include <runtime/Context.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/CompilerParser.h>
#include <toolchain/CompilerToolchain.h>
#include <toolchain/AssemblerToolchain.h>
#include <toolchain/Linker.h>
#include <utility/ExitProgramException.h>
#include <validator/X86BuilderValidator.h>
#include <validator/TypeValidator.h>
#include <builder/PEBuilder.h>
#include <builder/BMBuilder.h>

// Haze Main.cpp
// (c) Connor J. Link. All Rights Reserved.

using namespace hz;

int main(int argc, char** argv)
{
	WebSocketServer server{ [](SOCKET client)
	{
		std::cout << "New client connected: " << client << std::endl;
	}, [](SOCKET client, const std::string& message)
	{
		std::cout << "Received message from client " << client << ": " << message << std::endl;
	}, [](SOCKET client)
	{
		std::cout << "Client disconnected: " << client << std::endl;
	}, [](const std::string& error)
	{
		std::cerr << "Error: " << error << std::endl;
	} };

	/*server.start(8080);
	std::cout << "WebSocket server started on port 8080." << std::endl;
	std::cin.get();*/

	// minimal global (thread-shared) startup for singletons required to parse command line arguments
	SingletonContainer::instance().register_singleton<ErrorReporter>();
	SingletonContainer::instance().register_singleton<CommandLineOptions>();
	SingletonContainer::instance().register_singleton<FileManager>();

	// spools up the worker thread in the background to idle until symbol information becomes available
	USE_UNSAFE(SymbolExporter)->launch();

	auto command_line_parser = CommandLineParser{};
	command_line_parser.parse(argc, argv);

	// offload the loading process now to a background thread, await after initializing the rest of the components
	auto loader = Task<std::unordered_map<std::string_view, File>>{ [&]()
	{	
		for (auto& filepath : command_line_parser.files())
		{
			if (USE_UNSAFE(FileManager)->has_file(filepath))
			{
				USE_UNSAFE(ErrorReporter)->post_warning(std::format(
					"file `{}` was already read"))
			}

			USE_UNSAFE(FileManager)->open_file(filepath);

			const auto& file = USE_UNSAFE(FileManager).get_file(filepath);
			result[filepath] = file.content();
		}

		co_return result;
	} };
	
	// register all other global (thread-shared) singletons
	SingletonContainer::instance().register_singleton<Context>();
	SingletonContainer::instance().register_singleton<SymbolExporter>(std::cout);
	SingletonContainer::instance().register_singleton<SymbolDatabase>();
	SingletonContainer::instance().register_singleton<Linker>();

	// unit testing requires some singletones to be registered
	const auto execution = USE_UNSAFE(CommandLineOptions)->execution;
	if (execution == ExecutionType::VALIDATE)
	{
		AutoJob test_task{ "unit testing" };

		auto validators = std::vector<Validator*>
		{
			new X86BuilderValidator{},
			new TypeValidator{},
		};

		for (auto validator : validators)
		{
			validator->run_tests();
		}

		USE_UNSAFE(ErrorReporter)->post_information(std::format(
			"validation complete"), NULL_TOKEN);
		return 0;
	}
	else if (execution != ExecutionType::COMPILE)
	{
		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"unrecognized execution type `{}`", std::to_underlying(execution)), NULL_TOKEN);
		return 1;
	}

	// enumerate system threads
	auto thread_count = static_cast<std::size_t>(std::thread::hardware_concurrency());
	// NOTE: hardware concurrency can return 0, default to 4 then
	thread_count = thread_count == 0 ? 4 : thread_count; 
	// no need to spawn more threads than there are files
	thread_count = std::min(thread_count, command_line_parser.files().size());

	using ThreadWork = std::vector<std::string>;

	// create the threadpool and pin work items 
	std::vector<ThreadWork> thread_work{};
	thread_work.reserve(thread_count);

	for (auto i = decltype(thread_count){ 0 }; i < thread_count; i++)
	{
		ThreadWork work{};

		for (auto j = i; j < command_line_parser.files().size(); j += thread_count)
		{
			work.push_back(command_line_parser.files()[j]);
		}

		thread_work.push_back(work);
	}


	// quiesce on file manager
	co_await loader;


	// run once per thread
	auto thread_initialization = [&]
	{
		const auto& file = USE_UNSAFE(FileManager)->get_file(filepath);

		ServiceContainer::instance().register_factory<JobManager>([]
		{
			return std::make_shared<JobManager>();
		});
	};

	// run once per translation unit
	auto thread_work = [&](const std::string& filepath)
	{
		const auto& file = USE_UNSAFE(FileManager)->get_file(filepath);

		ServiceContainer::instance().register_factory<Generator>([]
		{
			return std::make_shared<Generator>(filepath);
		});
		ServiceContainer::instance().register_factory<RuntimeAllocator>([]
		{
			return std::make_shared<RuntimeAllocator>();
		});
		switch (file.ttype())
		{
			case ToolchainType::ASSEMBLER:
			{
				ServiceContainer::instance().register_factory<Parser, AssemblerParser>([&]
				{
					return std::make_shared<AssemblerParser>(filepath);
				});
				ServiceContainer::instance().register_factory<Toolchain, AssemblerToolchain>([&]
				{
					return std::make_shared<AssemblerToolchain>();
				});
			} break;

			case ToolchainType::COMPILER:
			{
				ServiceContainer::instance().register_factory<Parser, CompilerParser>([&]
				{
					return std::make_shared<CompilerParser>(filepath);
				});
				ServiceContainer::instance().register_factory<Toolchain, CompilerToolchain>([&]
				{
					return std::make_shared<CompilerToolchain>();
				});
			} break;

			default:
			{
				USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
					"unrecognized toolchain type for file `{}`", filepath), NULL_TOKEN);
			} break;
		}

		try
		{
			REQUIRE_UNSAFE(Toolchain)->run(filepath);
		}
		catch (ExitProgramException e)
		{
			USE_UNSAFE(ErrorReporter)->post_information(e.what(), NULL_TOKEN);
			// graceful shutdown for this reason
			REQUIRE_UNSAFE(Toolchain)->shut_down(false);
		}
		catch (std::exception e)
		{
			USE_UNSAFE(ErrorReporter)->post_uncorrectable(e.what(), NULL_TOKEN);
			REQUIRE_UNSAFE(Toolchain)->panic();
		}
	};


	// launch the thread pool
	std::vector<std::thread> threads{};
	for (auto& work : thread_work)
	{
		threads.emplace_back([&, work = std::move(work)]
		{
			thread_initialization();

			for (auto& filepath : work)
			{
				thread_work(filepath);
			}
		});
	}

	// block wait until all threads are complete to start linking
	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}	

#pragma message("TODO: figure out how to reload the linker with the new linkables from the symbol database")
	USE_UNSAFE(Linker)->reload();

	const auto link_task = REQUIRE_SAFE(JobManager)->begin_job("linking");

	const auto origin = REQUIRE_UNSAFE(Generator)->resolve_origin();
	const auto commands = USE_UNSAFE(Linker)->link(origin, UDWORD_MAX);

	const auto executable = commands
		| std::ranges::views::transform([](auto command) { return command->object_code; })
		| std::ranges::views::join
		| std::ranges::to<ByteRange>();

	REQUIRE_UNSAFE(JobManager)->end_job(link_task);



	const auto entrypoint = get_linker_origin(USE_UNSAFE(CommandLineOptions)->architecture);

	const auto finalize_task = REQUIRE_UNSAFE(JobManager)->begin_job("finalizing");

#pragma message("TODO: for now only one translation unit is supported per executable")

	auto output_filepath = USE_UNSAFE(CommandLineOptions)->output_filepath;
	if (output_filepath.empty())
	{
		output_filepath = "my_program";

		USE_UNSAFE(ErrorReporter)->post_warning(std::format(
			"no output filepath specified, defaulting to `{}`", output_filepath), NULL_TOKEN);
	}

	const auto fullpath = std::filesystem::path(output_filepath);
	const auto basename = fullpath.stem().string();

	// for x86, it is desirable to test the raw object code without PE formatting
	if (USE_UNSAFE(CommandLineOptions)->raw_output)
	{
		const auto filepath_out = std::format("{}.hzb", basename);

		auto outfile = std::ofstream(filepath_out, std::ios::out | std::ios::binary);

		if (!outfile)
		{
			USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
				"output file `{}` not writable", filepath_out), NULL_TOKEN);
		}

		outfile.write(reinterpret_cast<const char*>(executable.data()), executable.size());
		outfile.close();
	}


	// export the linked executable for the target architecture
	switch (USE_UNSAFE(CommandLineOptions)->architecture)
	{
		case ArchitectureType::X86:
		{
			auto pe_builder = new PEBuilder{ executable };
			output_filepath = std::format("{}.exe", basename);
			pe_builder->export_exe(output_filepath, basename);
		} break;

		case ArchitectureType::RISCV:
		{
			auto bm_builder = new BMBuilder{ executable };
			output_filepath = std::format("{}.bin", basename);
			bm_builder->export_exe(output_filepath, basename);
		} break;

		default:
		{
			USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
				"unrecognized architecture type `{}`", std::to_underlying(USE_UNSAFE(CommandLineOptions)->architecture)), NULL_TOKEN);
		} break;
	}

	USE_UNSAFE(ErrorReporter)->post_information(std::format(
		"wrote fresh executable for `{}`", output_filepath), NULL_TOKEN);
	
	REQUIRE_UNSAFE(JobManager)->end_job(finalize_task);
	
	return 0;
}
