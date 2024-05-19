#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include <string>
#include <vector>

#include "Token.h"

namespace hz
{
    class Lexer
    {
    private:
        std::string input;

    private:
        std::vector<Token> tokens;
        std::size_t line = 0;

    public:
        Lexer(std::string&& input)
            : input(std::move(input))
        {
        }

        std::vector<Token> lex();
    };
}

#endif //HAZE_LEXER_H