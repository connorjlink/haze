#include "Preprocessor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Log.h"
#include "Generator.h"
#include "Allocator.h"
#include "Simulator.h"

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <fstream>
#include <format>

hz::Allocator* hz::allocator;
hz::Generator* hz::generator;
hz::Parser* hz::parser;


int main(int argc, char** argv)
{
    std::string filepath = argv[1];

    if (argc != 2 || (filepath.substr(filepath.length() - 2) != "hz"))
    {
        hz::Log::info("correct usage is 'haze file.hz'");
        return EXIT_FAILURE;
    }

    std::ifstream file(filepath);

    if (!file.good())
    {
        hz::Log::error(std::format("the file {} could not be found", filepath));
    }

    //Create our static and dynamic allocation manager before everything else
    hz::allocator = new hz::Allocator;

    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());

    hz::Preprocessor preprocessor{ std::move(contents), filepath };
    preprocessor.preprocess();

    hz::Lexer lexer{ std::move(contents) };
    auto tokens = lexer.lex();

    hz::parser = new hz::CompilerParser{ std::move(tokens) };
    auto ast = hz::parser->parse();

    hz::generator = new hz::Generator{ std::move(ast) };
    hz::generator->generate();

    hz::Log::info(std::format("successfully compiled {}", filepath));


    //REDO This since we changed the opcode mapping a bit
    std::vector<std::uint8_t> rom = 
    { 
        0x10, 0x07, 0x00, //load x, #7
        0x14, 0x04, 0x00, //load y, #4
        0x41, 0x00, 0x00, //iadd x, y
        0x94, 0x00, 0x00, //bnot y
        0xC0, 0x00, 0x00, //push x
        0xC4, 0x00, 0x00, //push y
        0xD0, 0x00, 0x00, //pull x
        0xD4, 0x00, 0x00, //pull y
    };


    hz::Simulator sim{ std::move(rom) };
    sim.reset();
    sim.run();

    return EXIT_SUCCESS;
}
