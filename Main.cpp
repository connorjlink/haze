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

#include "Toolchain.h"
#include "CompilerToolchain.h"
#include "AssemblerToolchain.h"
#include "InterpreterToolchain.h"

#include <cstdlib>
#include <string>
#include <fstream>
#include <filesystem>
#include <format>
#include <chrono>

using namespace hz;

Allocator* hz::_allocator;
Generator* hz::_generator;
Parser* hz::_parser;
Context* hz::_context;
Toolchain* hz::_toolchain;
ErrorReporter* hz::_error_reporter;
JobManager* hz::_job_manager;

namespace mqtt
{
	const int message::DFLT_QOS = 0;
	const bool message::DFLT_RETAINED = true;
}

int main(int argc, char** argv)
{
	_allocator = new Allocator{};
	_context = new Context{};

	_error_reporter = new ErrorReporter{};
	_job_manager = new JobManager{};


	auto command_line_parser = CommandLineParser{};
	*_options = command_line_parser.parse(argc, argv);

	if (argc != 2)
	{
		Log::error("correct usage is 'haze' *.hz[x]");
	}

	//const auto path = std::filesystem::path(argv[1]);
	//const auto path = std::filesystem::path("./common.hz");
	//const auto path = std::filesystem::path("test.hzs");
	//const auto path = std::filesystem::path("test.hz");
	//const auto path = std::filesystem::path("sample.hzs");
	//const auto path = std::filesystem::path("test.hzi");
	const auto path = std::filesystem::path("sample.hzi");


	const auto filepath = path.string();
	const auto filename = path.filename().string();
	const auto extension = path.extension().string();

	const auto filesize = std::filesystem::file_size(filepath);

	auto file = std::ifstream(path);

	if (!file.good())
	{
		Log::error(std::format("the file {} could not be opened", filepath));
	}

	


	if (extension == ".hz")
	{
		// Compiler
		_toolchain = new CompilerToolchain{};
		_toolchain->init({ filepath });
	}

	else if (extension == ".hzs")
	{
		// Assembler

		//We are trying to assemble a program
		/*_parser = new AssemblerParser{ std::move(tokens) };

		auto commands = _parser->parse();

		linker = new AssemblerLinker{ std::move(commands), AS_ASSEMBLER_PARSER(_parser) };*/
	}

	else if (extension == ".hzi")
	{
		// Interpreter
		_toolchain = new InterpreterToolchain{};
		_toolchain->init({ filepath });
	}

	else
	{
		Log::error(std::format("unrecognized file extension {}", extension));
	}


	// Interpreter context will never fall through to here

	Linker* linker = nullptr;
 	auto image = linker->link(HALF_DWORD_MAX);

	auto emitter = new HazeEmitter{ std::move(image) };
	//auto emitter = new X86Emitter{ std::move(image) };

	auto executable = emitter->emit();


	auto end_time = std::chrono::steady_clock::now();
	auto compile_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	Log::info(std::format("successfully compiled {} in {}", filepath, compile_time));

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

	Simulator sim{ std::move(executable) };
	sim.reset();
	sim.run();

	return EXIT_SUCCESS;
}
