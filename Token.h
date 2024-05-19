#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <cstdlib>
#include <string>
#include <optional>
#include <unordered_map>

namespace hz
{
    enum class TokenType
    {
        INT,

        IDENTIFIER,

        FUNCTION,
        BYTE,
        RETURN,

        SEMICOLON,
        COMMA,
        EQUALS,

        LPAREN, RPAREN,
        LBRACE, RBRACE,

        PLUS, MINUS, STAR,

        END,
    };

    struct Token
    {
        TokenType type;
        std::size_t line;
        std::optional<std::string> value;
    };

    static const std::unordered_map<TokenType, const char*> debug_tokens
    {
        { TokenType::INT, "int" },
        { TokenType::IDENTIFIER, "identifier" },

        { TokenType::FUNCTION, "function" },
        { TokenType::BYTE, "byte" },

        { TokenType::SEMICOLON, ";" },
        { TokenType::COMMA, "," },
        { TokenType::EQUALS, "=" },

        { TokenType::LPAREN, "(" },
        { TokenType::RPAREN, ")" },
        { TokenType::LBRACE, "{" },
        { TokenType::RBRACE, "}" },

        { TokenType::PLUS, "+" },
        { TokenType::MINUS, "-" },
        { TokenType::STAR, "*" },

        { TokenType::END, "eof" },
    };
}

#endif //HAZE_TOKEN_H
