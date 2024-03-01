#include "Lexer.h"
#include "Log.h"

#include <iostream>
#include <fmt/format.h>

namespace hz
{
    void Lexer::lex()
    {
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
                case '=': tokens.emplace_back(Token{ EQUALS, line }); break;
                case ';': tokens.emplace_back(Token{ SEMICOLON, line }); break;
                case ',': tokens.emplace_back(Token{ COMMA, line }); break;

                case '(': tokens.emplace_back(Token{ LPAREN, line }); break;
                case ')': tokens.emplace_back(Token{ RPAREN, line }); break;
                case '{': tokens.emplace_back(Token{ LBRACE, line }); break;
                case '}': tokens.emplace_back(Token{ RBRACE, line }); break;

                case '+': tokens.emplace_back(Token{ PLUS, line }); break;
                case '-': tokens.emplace_back(Token{ MINUS, line }); break;
                case '*': tokens.emplace_back(Token{ STAR, line }); break;

                case 'a'...'z': [[fallthrough]];
                case 'A'...'Z':
                {
                    auto lexeme = rest(std::isalnum);

                    if (lexeme == "byte")
                    {
                        tokens.emplace_back(Token{ BYTE, line });
                    }

                    else if (lexeme == "function")
                    {
                        tokens.emplace_back(Token{ FUNCTION, line });
                    }

                    else if (lexeme == "return")
                    {
                        tokens.emplace_back(Token{ RETURN, line });
                    }

                    else if (lexeme == "true")
                    {
                        tokens.emplace_back(Token{ INT, line, "1" });
                    }

                    else if (lexeme == "false")
                    {
                        tokens.emplace_back(Token{ INT, line, "0 " });
                    }

                    else
                    {
                        tokens.emplace_back(Token{ IDENTIFIER, line, lexeme });
                    }
                } break;

                case '0'...'9':
                {
                    tokens.emplace_back(Token{ INT, line, rest(std::isdigit) });
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
                    }

                    else goto error;
                }

                default:
                {
                error:
                    Log::error(fmt::format("({}) unexpected character {}", line, input[i]));
                }
            }
        }

        tokens.emplace_back(Token{ TokenType::END });
    }

    void Lexer::print_tokens()
    {
        for (auto& token: tokens)
        {
            std::cout << fmt::format("Token: {}", hz::debug_tokens.at(token.type));

            if ((token.type == hz::TokenType::INT || token.type == hz::TokenType::IDENTIFIER) && token.value.has_value())
            {
                std::cout << fmt::format(" ({})", token.value.value());
            }

            std::cout << std::endl;
        }
    }
}
