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

namespace
{
	consteval auto formulate(Opcode opcode, Register operand1, Register operand2)
    {
        return (opcode << 4) | (operand1 << 2) | (operand2 << 0);
	}
}

int main(int argc, char** argv)
{
    std::string filepath = argv[1];

    if (argc != 2 || (filepath.substr(filepath.length() - 2) != "hz"))
    {
        Log::info("correct usage is 'haze file.hz'");
        return EXIT_FAILURE;
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

    generator = new Generator{ std::move(ast) };
    auto code = generator->generate();

    Log::info(std::format("successfully compiled {}", filepath));

    //REDO This since we changed the opcode mapping a bit
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


    Simulator sim{ std::move(rom) };
    sim.reset();
    sim.run();

    return EXIT_SUCCESS;
}
