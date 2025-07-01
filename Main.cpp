import std;

#include "SymbolDatabase.h"
#include "SymbolExporter.h"
#include "Context.h"
#include "AutoJob.h"
#include "JobManager.h"
#include "FileManager.h"
#include "CompilerToolchain.h"
#include "AssemblerToolchain.h"
#include "InterpreterToolchain.h"
#include "CommandLineOptions.h"
#include "CommandLineParser.h"
#include "CompilerParser.h"
#include "X86BuilderValidator.h"
#include "ErrorReporter.h"
#include "HeapAllocator.h"
#include "StackAllocator.h"
#include "RuntimeAllocator.h"
#include "ExitProgramException.h"
#include "DependencyInjector.h"

// Haze Main.cpp
// (c) Connor J. Link. All Rights Reserved.

using namespace hz;

// NOTE: used for the MQTT client hook
//namespace mqtt
//{
//	const int message::DFLT_QOS = 0;
//	const bool message::DFLT_RETAINED = true;
//}

int main(int argc, char** argv)
{
	// global (thread-shared) singleton startup
	SingletonContainer::instance().register_singleton<ErrorReporter>();
	SingletonContainer::instance().register_singleton<FileManager>();
	SingletonContainer::instance().register_singleton<Context>();
	SingletonContainer::instance().register_singleton<SymbolExporter>(std::cout);
	SingletonContainer::instance().register_singleton<SymbolDatabase>();
	SingletonContainer::instance().register_singleton<CommandLineOptions>();

	// spools up the worker thread in the background to idle until symbol information becomes available
	USE_UNSAFE(SymbolExporter)->launch();

	auto command_line_parser = CommandLineParser{};
	command_line_parser.parse(argc, argv);
	
	// require explicit opt-in to run tests; takes about 300us otherwise
	if (USE_UNSAFE(CommandLineOptions)->_execution == ExecutionType::VALIDATE)
	{
		AutoJob test_task{ "unit testing" };

		auto validators = std::vector<Validator*>
		{
			new X86BuilderValidator{},
		};

		for (auto validator : validators)
		{
			validator->run_tests();
		}
	}

	for (auto& filepath : command_line_parser.files())
	{
		// TODO: break specified files into workgroups to spawn a thread pool

		try
		{
			auto& file_manager = *USE_UNSAFE(FileManager);
			file_manager.open_file(filepath);

			const auto& file = file_manager.get_file(filepath);

			// TODO: instance (thread-local) services startup
			ServiceContainer::instance().register_factory<JobManager>([]
			{
				return std::make_shared<JobManager>();
			});
			ServiceContainer::instance().register_factory<Generator>([&]
			{
				return std::make_shared<Generator>(filepath);
			});
			ServiceContainer::instance().register_factory<RuntimeAllocator>([&]
			{
				return std::make_shared<RuntimeAllocator>();
			});
			ServiceContainer::instance().register_factory<StackAllocator>([&]
			{
				return std::make_shared<StackAllocator>();
			});
			ServiceContainer::instance().register_factory<HeapAllocator>([&]
			{
				return std::make_shared<HeapAllocator>();
			});
#pragma message ("TODO: refactor the pipeline dependency system so I can safely create the correct type before it is needed. now, this is a dummy just so Toolchain doesn't try to create one/misses injection")
			ServiceContainer::instance().register_factory<Parser>([&]
			{
				return std::make_shared<CompilerParser>(filepath);
			});


			switch (file.ttype())
			{
#pragma message ("TODO: pass along the proper argument to each toolchain type, e.g., filepath for split compilation")


				case ToolchainType::ASSEMBLER:
				{
					ServiceContainer::instance().register_factory<Toolchain>([]
					{
						return std::make_shared<AssemblerToolchain>();
					});
				} break;

				case ToolchainType::COMPILER:
				{
					ServiceContainer::instance().register_factory<Toolchain>([]
					{
						return std::make_shared<CompilerToolchain>();
					});
				} break;

				case ToolchainType::INTERPRETER:
				{
					ServiceContainer::instance().register_factory<Toolchain>([]
					{
						return std::make_shared<InterpreterToolchain>();
					});
				} break;
			}

			REQUIRE_UNSAFE(Toolchain)->init(filepath);
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
	}

	/*consteval auto formulate = [](auto opcode, auto operand1, auto operand2)
	{
		return (opcode << 4) | (operand1 << 2) | (operand2 << 0);
	};
	std::vector<std::uint8_t> rom = 
	{
		::formulate(COPY, R0, DC), 0x7, 0x0, //load r0, #7
		::formulate(COPY, R1, DC), 0x4, 0x0, //load r1, #4
		::formulate(IADD, R0, R1), 0x0, 0x0, //iadd r0, r1
		::formulate(BXOR, R1, R1), 0x0, 0x0, //bnot r1
		::formulate(PUSH, DC, R0), 0x0, 0x0, //push r0
		::formulate(PUSH, DC, R1), 0x0, 0x0, //push r1
		::formulate(PULL, R0, DC), 0x0, 0x0, //pull r0
		::formulate(PULL, R1, DC), 0x0, 0x0, //pull r1
	};
	Simulator sim{ std::move(rom) };*/

	return 0;
}
