import std;

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX

#include <Task.h>
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
#include <toolchain/defs/ToolchainKind.h>
#include <utility/Exception.h>
#include <builder/PEBuilder.h>
#include <builder/BMBuilder.h>

// Haze Main.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	Task<std::unordered_map<std::string, std::string>> load_files_async(CommandLineParser& command_line_parser)
	{
		std::unordered_map<std::string, std::string> result{};

		for (auto& filepath : command_line_parser.files())
		{
			if (USE_UNSAFE(FileManager)->has_file(filepath))
			{
				USE_UNSAFE(ErrorReporter)->post_warning(std::format(
					"file `{}` was already read", filepath), NULL_TOKEN);
			}

			USE_UNSAFE(FileManager)->open_file(filepath);

			const auto& file = USE_UNSAFE(FileManager)->get_file(filepath);
			result[filepath] = file.get_processed_contents();
		}

		co_return result;
	}
}

hz::Task<int> main_shim(int argc, char** argv)
{
	using namespace hz;

	WebSocketServer server{ [](SOCKET client)
	{
		std::cerr << std::format("New client connected: {}", client);
	}, [](SOCKET client, const std::string& message)
	{
		std::cerr << std::format("Received message from client {}: {}", client, message);
	}, [](SOCKET client)
	{
		std::cerr << std::format("Client disconnected: {}", client);
	}, [](const std::string& error)
	{
		std::cerr << std::format("WebSocketServer error: {}", error);
	} };

	/*server.start(8080);
	std::cerr << std::format("WebSocket server started on port 8080.");
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
	auto loader = load_files_async(command_line_parser);

	// register all other global (thread-shared) singletons
	SingletonContainer::instance().register_singleton<Context>();
	SingletonContainer::instance().register_singleton<SymbolExporter>(std::cout);
	SingletonContainer::instance().register_singleton<SymbolDatabase>();
	SingletonContainer::instance().register_singleton<Linker>();

	// enumerate system threads
	auto thread_count = static_cast<std::size_t>(std::thread::hardware_concurrency());
	// NOTE: hardware concurrency can return 0, default to 4 then
	thread_count = thread_count == 0 ? 4 : thread_count;
	// no need to spawn more threads than there are files
	thread_count = std::min(thread_count, command_line_parser.files().size());


	// create the threadpool and pin work items 
	using ThreadWork = std::vector<std::string>;
	std::vector<ThreadWork> thread_work{};
	thread_work.reserve(thread_count);

	for (auto i = 0uz; i < thread_count; i++)
	{
		ThreadWork work{};

		for (auto j = i; j < command_line_parser.files().size(); j += thread_count)
		{
			work.push_back(command_line_parser.files()[j]);
		}

		thread_work.push_back(work);
	}


	// run once per thread
	auto thread_initialization = [&]
	{
		ServiceContainer::instance().register_factory<JobManager>([]
		{
			return std::make_shared<JobManager>();
		});
	};

	// run once per translation unit
	auto thread_translation = [&](const std::string& filepath)
	{
		const auto& file = USE_UNSAFE(FileManager)->get_file(filepath);

		ServiceContainer::instance().register_factory<Generator>([]
		{
			return std::make_shared<Generator>(filepath);
		});
		ServiceContainer::instance().register_factory<Allocator>([]
		{
			return std::make_shared<Allocator>();
		});

		switch (file.tag_type())
		{
			case ToolchainKind::ASSEMBLER:
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

			case ToolchainKind::COMPILER:
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
		catch (const ExitProgramException& exception)
		{
			USE_UNSAFE(ErrorReporter)->post_information(exception.what(), NULL_TOKEN);
			// graceful shutdown for this reason
			REQUIRE_UNSAFE(Toolchain)->shut_down(false);
		}
		catch (const std::exception& exception)
		{
			USE_UNSAFE(ErrorReporter)->post_uncorrectable(exception.what(), NULL_TOKEN);
			// uh oh!
			REQUIRE_UNSAFE(Toolchain)->panic();
		}
	};

	// quiesce on file manager
	co_await loader;


	// launch the thread pool
	std::vector<std::thread> threads{};
	for (auto& work : thread_work)
	{
		threads.emplace_back([&, work = std::move(work)]
		{
			thread_initialization();

			for (auto& filepath : work)
			{
				thread_translation(filepath);
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

	const auto architecture = USE_UNSAFE(CommandLineOptions)->architecture;

#pragma message("TODO: figure out how to reload the linker with the new linkables from the symbol database")
	USE_UNSAFE(Linker)->reload();

	const auto link_task = REQUIRE_SAFE(JobManager)->begin_job("linking");

	const auto origin = REQUIRE_UNSAFE(Generator)->resolve_origin();
	const auto base = get_linker_origin(architecture);

	const auto commands = USE_UNSAFE(Linker)->link(origin, base);

	const auto executable = commands
		| TypeHandle get_type(const TypeStorage&) const;::transform([](auto command) { return command->object_code; })
		| TypeHandle get_type(const TypeStorage&) const;::join
		| std::ranges::to<ByteRange>();

	REQUIRE_UNSAFE(JobManager)->end_job(link_task);



	const auto entrypoint = get_linker_origin(architecture);

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
		case ArchitectureKind::X86:
		{
			auto pe_builder = new PEBuilder{ executable };
			output_filepath = std::format("{}.exe", basename);
			pe_builder->export_exe(output_filepath, basename);
		} break;

		case ArchitectureKind::RISCV:
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

	co_return 0;
}

int main(int argc, char** argv)
{
	return main_shim(argc, argv).get();
}
