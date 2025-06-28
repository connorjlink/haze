import std;

#include "Lexer.h"
#include "ErrorReporter.h"

// Haze Lexer.cpp
// (c) Connor J. Link. All Rights Reserved.

#define APPEND_TOKEN(type, raw) tokens.emplace_back(Token{ type, raw, { _filepath, _position, _line, _column } })

namespace
{
	// used to continue lexing until reaching the end of a string literal
	// NOTE: this method does not work if there are escape code \" characters embedded within!
	int isquoted(int c)
	{
		return c != '"';
	}

	// used to continue lexing until reaching a word boundary
	// NOTE: this method is naive and only checks for whitespace characters
	int nisspace(int c)
	{
		// NOTE: boolean NOT is the intended behavior here
		return !std::isspace(c);
	}


	hz::Token error_token(const std::string& value, hz::SourceLocation location)
	{
		hz::Token token{};

		token.type = hz::TokenType::ERROR;
		token.text = value;
		token.location = location;
		
		return token;
	}
}

namespace hz
{
	void Lexer::advance(std::size_t how_many)
	{

	}

	void Lexer::expect()
	{

	}

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

			const auto& current = _input[i + 0];
			const auto& next = _input[i + 1];

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
					::error_token({ current }, { _filepath, _position, _line, _column }));
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
				// skip opening quote
				i++;

				const auto lexeme = rest(::isquoted);

				// skip closing quote
				i++;

				APPEND_TOKEN(STRING, lexeme);
			}

			else if (current == '=')
			{
				if (next == '=')
				{
					const auto lexeme = "==";
					APPEND_TOKEN(EQUALSEQUALS, lexeme);
					
					i++;
				}
				
				else
				{
					const auto lexeme = "=";
					APPEND_TOKEN(EQUALS, lexeme);
				}
			}

			else if (current == '!')
			{
				if (next == '=')
				{
					const auto lexeme = "!=";
					APPEND_TOKEN(EXCLAMATIONEQUALS, lexeme);

					i++;
				}

				else
				{
					const auto lexeme = "!";
					APPEND_TOKEN(EXCLAMATION, lexeme);
				}
			}

			else
			{
				const auto current_string = std::string{ current };

				const auto search = _token_map.at(current_string);

				if (search.has_value())
				{
					APPEND_TOKEN(search.value(), current_string);
				}

				else
				{
					_error_reporter->post_error(std::format("unexpected token `{}`", current_string),
						::error_token(current_string, { _filepath, _position, _line, _column }));
				}
			}
		}

		APPEND_TOKEN(TokenType::END, "eof");

		return tokens;
	}
}
