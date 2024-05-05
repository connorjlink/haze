#include "Driver.h"

#include "Preprocessor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Optimizer.h"
#include "Generator.h"
#include "Reducer.h"
#include "Simulator.h"

namespace hz
{
    Driver::Driver(std::string file, std::string_view filepath)
    {
        preprocessor = new Preprocessor(file, filepath);
        preprocessor->preprocess();

        lexer = new Lexer{ file };
        lexer->lex();

        parser = new Parser{ lexer->get_tokens() };
        parser->parse();

        optimizer = new Optimizer{ parser->get_ast(), parser, OPTIMIZE_AST | 0 };
        optimizer->optimize();

        generator = new Generator{ optimizer->get_ast() };

        reducer = new Reducer{ generator->get_bytes() };
        reducer->reduce();

        simulator = new Simulator{ reducer->get_bytes() };
        simulator->reset();
        simulator->run();
    }
}