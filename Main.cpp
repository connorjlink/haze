#include "Allocator.h"
#include "Preprocessor.h"
#include "Lexer.h"
#include "CompilerParser.h"
#include "AssemblerParser.h"
#include "Generator.h"
#include "Linker.h"
#include "Simulator.h"
#include "Log.h"

#include <cstdlib>
#include <string>
#include <fstream>
#include <filesystem>
#include <format>
#include <chrono>

using namespace hz;

Allocator* hz::allocator;
Generator* hz::generator;
Parser* hz::parser;

int main(int argc, char** argv)
{
	auto start_time = std::chrono::steady_clock::now();

	if (argc != 2)
	{
		Log::error("correct usage is 'haze' *.hz(s)");
	}

	const auto path = std::filesystem::path(argv[1]);

	const auto filepath = path.string();
	const auto filename = path.filename().string();
	const auto extension = path.extension().string();

	const auto filesize = std::filesystem::file_size(filepath);

	auto file = std::ifstream(path);

	if (!file.good())
	{
		Log::error(std::format("the file {} could not be opened", filepath));
	}

	std::string source_code(filesize, '\0');
	source_code.assign((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());

	allocator = new Allocator{};

	auto preprocessor = new Preprocessor{ std::move(source_code), filepath };
	auto processed = preprocessor->preprocess();

	auto lexer = new Lexer{ std::move(processed) };
	auto tokens = lexer->lex();

	if (extension == ".hz")
	{
		//We are trying to compile a program
		parser = new CompilerParser{ std::move(tokens) };
	}

	else if (extension == ".hzs")
	{
		//We are trying to assemble a program
		parser = new AssemblerParser{ std::move(tokens) };
	}

	else
	{
		Log::error(std::format("unrecognized file extension {}", extension));
	}

	auto ast = parser->parse();

	generator = new Generator{ std::move(ast) };
	auto linkables = generator->generate();

	auto linker = new Linker{ std::move(linkables) };
	auto executable = linker->link();


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
