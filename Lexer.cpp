import std;

#include "Lexer.h"
#include "ErrorReporter.h"

// Haze Lexer.cpp
// (c) Connor J. Link. All Rights Reserved.

#define APPEND_TOKEN(type, raw) tokens.emplace_back(Token{ type, raw, _line, _column })

namespace
{
	// used to continue parsing until reaching the end of a string literal
	// NOTE: this method does not work if there are escape code \" characters embedded within!
	int isquoted(int c)
	{
		return c != '"';
	}


	hz::Token error_token(std::string value, std::int16_t line, std::int16_t column)
	{
		hz::Token token{};

		token.value = value;
		token.line = line;
		token.column = column;

		return token;
	}
}

namespace hz
{
	std::vector<Token> Lexer::lex()
	{
		std::vector<Token> tokens{};

		/*
		std::cout << std::format("Token: {} ({})\n", hz::debug_tokens.at(token.type),
			(token.type == TokenType::INT || token.type == TokenType::IDENTIFIER) ? std::format("({})", token.value.value_or("undef")) : "");
		*/

		for (auto i = 0; i < _input.length(); i++)
		{
			auto rest = [&](int(*functor)(int))
			{
				std::string out;

				while (i < _input.length() && functor(_input[i]))
				{
					out += _input[i];
					i++;
				}

				i--;

				return out;
			};


			using enum TokenType;
			const auto& current = _input[i];

			if (current == ' ' || current == '\n' || current == '\r' || current == '\t')
			{
				if (current == '\n')
				{
					_line++;
					_column = 1;
				}

				continue;
			}

			else if (current == '/')
			{
				if (_input[i + 1] == '/')
				{
					while (_input[i] != '\n')
					{
						i++;
					}

					continue;
				}

				
				_error_reporter->post_error("unexpected character `/`", 
					::error_token({ current }, _line, _column));
			}

			else if ('0' <= current && current <= '9')
			{
				APPEND_TOKEN(INT, rest(std::isdigit));
			}

			else if (current == '$')
			{
				// Hexadecimal integer literal
				i++;
				const auto lexeme = rest(std::isxdigit);
				const auto number = std::stoi(lexeme, nullptr, 16);
				APPEND_TOKEN(INT, std::format("{}", number));
			}

			else if ('a' <= current && current <= 'z' ||
					 'A' <= current && current <= 'Z')
			{
				const auto lexeme = rest(std::isalnum);
				const auto search = _token_map.at(lexeme);

				if (search)
				{
					APPEND_TOKEN(*search, lexeme);
				}

				else
				{
					APPEND_TOKEN(IDENTIFIER, lexeme);
				}
			}

			else if (current == '.')
			{
				i++;
				const auto lexeme = rest(std::isalnum);
				const auto search = _token_map.at(std::format(".{}", lexeme));

				if (search)
				{
					APPEND_TOKEN(*search, lexeme);
				}

				else
				{
					APPEND_TOKEN(IDENTIFIER, lexeme);
				}
			}

			else if (current == '"')
			{
				i++;

				const auto lexeme = rest(::isquoted);

				i++;

				APPEND_TOKEN(STRING, lexeme);
			}

			else
			{
				const auto current_string = std::string{ current };

				const auto search = _token_map.at(current_string);

				if (search.has_value())
				{
					APPEND_TOKEN(search.value(), std::string{ current });
				}

				else
				{
					_error_reporter->post_error(std::format("unexpected character `{}`", current),
						::error_token(std::string{ current }, _line, _column));
				}
			}
		}

		APPEND_TOKEN(TokenType::END, "eof");

		return tokens;
	}
}
