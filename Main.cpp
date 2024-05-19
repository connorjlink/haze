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

    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());

    hz::Preprocessor preprocessor(contents, filepath);
    preprocessor.preprocess();

    hz::Lexer lexer(contents);
    lexer.lex();

    //lexer.print_tokens();

    hz::Parser parser(lexer.get_tokens());
    parser.parse();

    hz::allocator = new hz::Allocator;

    //hz::Generator generator(parser.get_ast());
    hz::generator = new hz::Generator(parser.get_ast());

    //generator.generate();

    //parser.print_ast();

    hz::Log::info(std::format("successfully compiled {}", filepath));


    std::vector<std::uint8_t> rom = {0x10, 0x07, 0x00, //load x, #7
                                           0x14, 0x04, 0x00, //load y, #4
                                           0x41, 0x00, 0x00, //iadd x, y
                                           0x94, 0x00, 0x00, //bnot y
                                           0xC0, 0x00, 0x00, //push x
                                           0xC4, 0x00, 0x00, //push y
                                           0xD0, 0x00, 0x00, //pull x
                                           0xD4, 0x00, 0x00, //pull y
    };


    hz::Simulator sim{ rom };
    sim.reset();
    sim.run();


    return EXIT_SUCCESS;
}
