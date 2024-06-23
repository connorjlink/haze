#include "Lexer.h"
#include "Log.h"

#include <iostream>
#include <format>

#define APPEND_TOKEN(x, raw) tokens.emplace_back(Token{ x, line, raw })

namespace hz
{
	std::vector<Token> Lexer::lex()
	{
		std::vector<Token> tokens{};

		/*
		std::cout << std::format("Token: {} ({})\n", hz::debug_tokens.at(token.type),
			(token.type == TokenType::INT || token.type == TokenType::IDENTIFIER) ? std::format("({})", token.value.value_or("undef")) : "");
		*/

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

				Log::error(std::format("({}) unexpected character `/`", line));
			}

			else if ('0' <= current && current <= '9')
			{
				APPEND_TOKEN(INT, rest(std::isdigit));
			}

			else if ('a' <= current && current <= 'z' ||
					 'A' <= current && current <= 'Z')
			{
				const auto lexeme = rest(std::isalnum);
				const auto search = lexeme_map.find(lexeme);

				if (search != std::end(lexeme_map))
				{
					APPEND_TOKEN(search->second, lexeme);
				}

				else
				{
					APPEND_TOKEN(IDENTIFIER, lexeme);
				}
			}

			else if (current == '.')
			{
				i++;
				const auto lexeme = rest(isalnum);
				const auto search = lexeme_map.find(std::format(".{}", lexeme));

				if (search != std::end(lexeme_map))
				{
					APPEND_TOKEN(search->second, lexeme);
				}

				else
				{
					APPEND_TOKEN(IDENTIFIER, lexeme);
				}
			}

			else
			{
				const auto search = lexeme_map.find(std::string{ current });

				if (search != std::end(lexeme_map))
				{
					APPEND_TOKEN(search->second, std::string{ current });
				}

				else
				{
					Log::error(std::format("({}) unexpected character `{}`", line, current));
				}
			}
		}

		APPEND_TOKEN(TokenType::END, "eof");

		return tokens;
	}
}
