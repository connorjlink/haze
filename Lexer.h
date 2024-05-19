#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include <cstdlib>
#include <string>
#include <vector>

#include "Token.h"

namespace hz
{
    class Lexer
    {
    private:
        const std::string& input;
        std::vector<Token> tokens;
        std::size_t line = 1;

    public:
        explicit Lexer(const std::string& input)
            : input(input)
        {
        }

    public:
        const std::vector<Token>& get_tokens() const
        {
            return tokens;
        }

    public:
        void print_tokens();
        void lex();
    };
}

#endif //HAZE_LEXER_H
