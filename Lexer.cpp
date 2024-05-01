#include "Lexer.h"
#include "Log.h"

#include <iostream>
#include <fmt/format.h>

namespace hz
{
    void Lexer::lex()
    {
#ifndef APPEND_TOKEN
#define APPEND_TOKEN(x) do { tokens.emplace_back(Token{ x, line }); break; } while (0)

#ifndef APPEND_TOKEN_VALUE
#define APPEND_TOKEN_VALUE(x, y) do { tokens.emplace_back(Token{ x, line, y }); } while (0)
        for (auto i = 0; i < input.length(); i++)
        {
            auto rest = [&](int(*functor)(int))
            {
                std::string out;

                while (i < input.length() && functor(input[i]))
                {
                    out += input[i];
                    i++;
                }

                i--;

                return out;
            };

            using enum TokenType;
            switch (input[i])
            {
                case '=': APPEND_TOKEN(EQUALS);
                case ';': APPEND_TOKEN(SEMICOLON);
                case ',': APPEND_TOKEN(COMMA);

                case '(': APPEND_TOKEN(LPAREN);
                case ')': APPEND_TOKEN(RPAREN);
                case '{': APPEND_TOKEN(LBRACE);
                case '}': APPEND_TOKEN(RBRACE);

                case '+': APPEND_TOKEN(PLUS);
                case '-': APPEND_TOKEN(MINUS);
                case '*': APPEND_TOKEN(STAR);

                case 'a'...'z': [[fallthrough]];
                case 'A'...'Z':
                {
                    const auto lexeme = rest(std::isalnum);

                         if (lexeme ==     "byte") APPEND_TOKEN(BYTE);
                    else if (lexeme == "function") APPEND_TOKEN(FUNCTION);
                    else if (lexeme ==   "return") APPEND_TOKEN(RETURN);
                    else if (lexeme ==     "true") APPEND_TOKEN_VALUE(INT, "1");
                    else if (lexeme ==    "false") APPEND_TOKEN_VALUE(INT, "0");
                    else                           APPEND_TOKEN_VALUE(IDENTIFIER, lexeme);
                } break;

                case '0'...'9':
                {
                    APPEND_TOKEN_VALUE(INT, rest(std::isdigit));
                } break;

                case '\n': line++; break;

                case ' ': [[fallthrough]];
                case '\t': break;

                case '/':
                {
                    if (input[i + 1] == '/')
                    {
                        while (input[i] != '\n')
                        {
                            i++;
                        }

                        break;
                    }
                }

                [[fallthrough]];
                default:
                {
                    Log::error(fmt::format("({}) unexpected character {}", line, input[i]));
                }
            }
        }

        APPEND_TOKEN(TokenType::END);

#undef APPEND_TOKEN_VALUE
#endif

#undef APPEND_TOKEN
#endif
    }

    void Lexer::print_tokens()
    {
        for (auto& token: tokens)
        {
            std::cout << fmt::format("Token: {} ({})\n", hz::debug_tokens.at(token.type),
                (token.type == TokenType::INT || token.type == TokenType::IDENTIFIER) ? fmt::format("({})", token.value.value_or("undef")) : "");
        }
    }
}
