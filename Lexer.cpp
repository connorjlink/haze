#include "Lexer.h"
#include "Log.h"

#include <iostream>
#include <format>

namespace hz
{
    void Lexer::lex()
    {
#ifndef APPEND_TOKEN
#define APPEND_TOKEN(x) tokens.emplace_back(Token{ x, line })

#ifndef APPEND_TOKEN_VALUE
#define APPEND_TOKEN_VALUE(x, y) tokens.emplace_back(Token{ x, line, y })
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
            const auto& current = input[i];

            if (current == ' ' || current == '\n' || current == '\r' || current == '\t')
            {
                if (current == '\n')
                {
                    line++;
                }

                continue;
            }

            else if (current == '/')
            {
                if (input[i + 1] == '/')
                {
                    while (input[i] != '\n')
                    {
                        i++;
                    }

                    continue;
                }

                Log::error("({}) unexpected character `/`");
            }

            else if ('0' < current && current < '9')
            {
                APPEND_TOKEN_VALUE(INT, rest(std::isdigit));
            }

            else if ('a' < current && current < 'z' ||
                'A' < current && current < 'Z')
            {
                const auto lexeme = rest(std::isalnum);
                const auto search = lexeme_map.find(lexeme);

                if (search != std::end(lexeme_map))
                {
                    APPEND_TOKEN(search->second);
                }
                else
                {
                    APPEND_TOKEN_VALUE(IDENTIFIER, lexeme);
                }
            }

            else
            {
                const auto search = lexeme_map.find(std::string{ current });

                if (search != std::end(lexeme_map))
                {
                    APPEND_TOKEN(search->second);
                }
                else
                {
                    Log::error(std::format("({}) unexpected character `{}`", line, current));
                }
            }


            using enum TokenType;
            switch (input[i])
            {
                case '=': APPEND_TOKEN(EQUALS); break;
                case ';': APPEND_TOKEN(SEMICOLON); break;
                case ',': APPEND_TOKEN(COMMA); break;

                case '(': APPEND_TOKEN(LPAREN); break;
                case ')': APPEND_TOKEN(RPAREN); break;
                case '{': APPEND_TOKEN(LBRACE); break;
                case '}': APPEND_TOKEN(RBRACE); break;

                case '+': APPEND_TOKEN(PLUS); break;
                case '-': APPEND_TOKEN(MINUS); break;
                case '*': APPEND_TOKEN(STAR); break;
                    
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
                    if (input[i] >= 'a' && input[i] <= 'z' ||
                        input[i] >= 'A' && input[i] <= 'Z')
                    {
                        const auto lexeme = rest(std::isalnum);

                             if (lexeme == "byte") APPEND_TOKEN(BYTE);
                        else if (lexeme == "function") APPEND_TOKEN(FUNCTION);
                        else if (lexeme == "return") APPEND_TOKEN(RETURN);
                        else if (lexeme == "true") APPEND_TOKEN_VALUE(INT, "1");
                        else if (lexeme == "false") APPEND_TOKEN_VALUE(INT, "0");
                        else                        APPEND_TOKEN_VALUE(IDENTIFIER, lexeme);

                        break;
                    }

                    else if (input[i] >= '0' && input[i] <= '9')
                    {
                        APPEND_TOKEN_VALUE(INT, rest(std::isdigit)); break;
                    }

                    else
                    {
                        Log::error(std::format("({}) unexpected character {}", line, input[i]));
                    }
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
            std::cout << std::format("Token: {} ({})\n", hz::debug_tokens.at(token.type),
                (token.type == TokenType::INT || token.type == TokenType::IDENTIFIER) ? std::format("({})", token.value.value_or("undef")) : "");
        }
    }
}
