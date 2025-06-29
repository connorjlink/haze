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

Context* hz::_context;
Toolchain* hz::_toolchain;

JobManager* hz::_job_manager;

CommandLineOptions* hz::_options;

HeapAllocator* hz::_heap_allocator;
StackAllocator* hz::_stack_allocator;

// NOTE: used for the MQTT client hook
//namespace mqtt
//{
//	const int message::DFLT_QOS = 0;
//	const bool message::DFLT_RETAINED = true;
//}

int main(int argc, char** argv)
{
	_database = new SymbolDatabase{};
	_exporter = new SymbolExporter{ std::cout };
	// spools up the worker thread in the background to idle until symbol information becomes available
	_exporter->launch();

	_heap_allocator = new HeapAllocator{};
	_stack_allocator = new StackAllocator{};
	_runtime_allocator = new RuntimeAllocator{};

	_context = new Context{};

	_job_manager = new JobManager{};

	_options = new CommandLineOptions{};

	auto command_line_parser = CommandLineParser{};
	command_line_parser.parse(argc, argv);


	hz::ServiceContainer::instance().register_factory<FileManager>([]
	{
		return std::make_shared<FileManager>();
	});

	// alternative API for thread-shared service (singleton)
	//hz::SingletonContainer::instance().register_singleton<FileManager>();
	hz::SingletonContainer::instance().register_singleton<ErrorReporter>();



	// require explicit opt-in to run tests; takes about 300us otherwise
	if (_options->_execution == ExecutionType::VALIDATE)
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
		try
		{
			auto& file_manager = USE_UNSAFE(FileManager);
			file_manager.open_file(filepath);

			const auto& file = file_manager.get_file(filepath);

			switch (file.ttype())
			{
				case ToolchainType::ASSEMBLER:
				{
					_toolchain = new AssemblerToolchain{};
				} break;

				case ToolchainType::COMPILER:
				{
					_toolchain = new CompilerToolchain{};
				} break;

				case ToolchainType::INTERPRETER:
				{
					_toolchain = new InterpreterToolchain{};
				} break;
			}

			_toolchain->init(filepath);
		}
		catch (ExitProgramException e)
		{
			USE_UNSAFE(ErrorReporter).post_information(e.what(), NULL_TOKEN);
			// graceful shutdown for this reason
			_toolchain->shut_down(false);
		}
		catch (std::exception e)
		{
			USE_UNSAFE(ErrorReporter).post_uncorrectable(e.what(), NULL_TOKEN);
			_toolchain->panic();
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
