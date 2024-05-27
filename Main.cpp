#include "Preprocessor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Log.h"
#include "Generator.h"
#include "Allocator.h"
#include "Simulator.h"

#include <cstdlib>
#include <string>
#include <fstream>
#include <format>

using namespace hz;

Allocator* hz::allocator;
Generator* hz::generator;
Parser* hz::parser;

int main(int argc, char** argv)
{
    static constexpr auto OPTIMIZE = true;
    static constexpr auto DEBUG = true;

    std::string filepath = argv[1];

    if (argc != 2 || (filepath.substr(filepath.length() - 2) != "hz"))
    {
        Log::error("correct usage is 'haze file.hz'");
    }

    std::ifstream file(filepath);

    if (!file.good())
    {
        Log::error(std::format("the file {} could not be found", filepath));
    }

    //Create our static and dynamic allocation manager before everything else
    allocator = new Allocator;

    std::string contents((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());

    auto preprocessor = new Preprocessor{ std::move(contents), filepath };
    auto processed = preprocessor->preprocess();

    auto lexer = new Lexer{ std::move(processed) };
    auto tokens = lexer->lex();

    parser = new CompilerParser{ std::move(tokens) };
    auto ast = parser->parse();

    if constexpr (OPTIMIZE)
    {
		for (auto& node : ast)
        {
	    	while (true)
	    	{
                if (auto node_optimized = node->optimize())
                {
                    node = node_optimized;
                }

                else
                {
	                break;
                }
	    	}
        }	    
    }

    generator = new Generator{ std::move(ast) };
    auto code = generator->generate();

    auto linker = new Linker{ std::move(code); };
    auto executable = linker->link();

    Log::info(std::format("successfully compiled {}", filepath));

    /*
    constexpr auto formulate = [](auto opcode, auto operand1, auto operand2)
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
    };*/


    // Simulator sim{ std::move(rom) };
    Simulator sim{ std::move(executable) };
    sim.reset();
    sim.run();

    return EXIT_SUCCESS;
}
