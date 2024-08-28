#include "Allocator.h"
#include "Preprocessor.h"
#include "Lexer.h"
#include "CompilerParser.h"
#include "AssemblerParser.h"
#include "InterpreterParser.h"
#include "Generator.h"
#include "Linker.h"
#include "CompilerLinker.h"
#include "AssemblerLinker.h"
#include "Simulator.h"
#include "Emitter.h"
#include "HazeEmitter.h"
#include "X86Emitter.h"
#include "Context.h"
#include "Log.h"
#include "CommandLineParser.h"
#include "Hook.h"
#include "JobManager.h"
#include "FileManager.h"
#include "Toolchain.h"
#include "CompilerToolchain.h"
#include "AssemblerToolchain.h"
#include "InterpreterToolchain.h"
#include "Simulator.h"
#include "ErrorReporter.h"
#include "PEBuilder.h"

#include <cstdlib>
#include <string>
#include <fstream>
#include <filesystem>
#include <format>
#include <chrono>
#include <exception>

using namespace hz;

Allocator* hz::_allocator;
Generator* hz::_generator;
Parser* hz::_parser;
Context* hz::_context;
Toolchain* hz::_toolchain;
Emitter* hz::_emitter;
Linker* hz::_linker;
ErrorReporter* hz::_error_reporter;
JobManager* hz::_job_manager;
FileManager* hz::_file_manager;
CommandLineOptions* hz::_options;
std::string hz::_current_file;

namespace mqtt
{
	const int message::DFLT_QOS = 0;
	const bool message::DFLT_RETAINED = true;
}

#include <fstream>

int main(int argc, char** argv)
{
	_allocator = new Allocator{};
	_context = new Context{};

	_error_reporter = new ErrorReporter{};
	_job_manager = new JobManager{};

	_options = new CommandLineOptions{};

	auto command_line_parser = CommandLineParser{};
	command_line_parser.parse(argc, argv);

	_file_manager = new FileManager{};
	_current_file = "";

	
	for (auto& filepath : command_line_parser.files())
	{
		try
		{
			_file_manager->open_file(filepath);
			const auto& file = _file_manager->get_file(filepath);
			_current_file = filepath;

			switch (file.ttype())
			{
				case ToolchainType::ASSEMBLER:
				{
					// Assembler
					_toolchain = new AssemblerToolchain{};
				} break;

				case ToolchainType::COMPILER:
				{
					// Compiler
					_toolchain = new CompilerToolchain{};
				} break;

				case ToolchainType::INTERPRETER:
				{
					// Interpreter
					_toolchain = new InterpreterToolchain{};
				} break;
			}

			_toolchain->init(filepath);
		}
		
		catch (std::exception)
		{
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

	return EXIT_SUCCESS;
}
