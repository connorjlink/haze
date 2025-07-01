import std;

#include "Lexer.h"
#include "ErrorReporter.h"

// Haze Lexer.cpp
// (c) Connor J. Link. All Rights Reserved.

#define APPEND_TOKEN(ttype, raw) tokens.emplace_back(Token{ .type = ttype, .text = raw, .location = get_state().location })

namespace hz
{
	Lexer::Lexer(const std::string& filepath)
		: Scanner{ filepath }
	{
		// transact an initial state for undoing
		commit<void>([&](auto& context) -> void
		{
			context.source = USE_SAFE(FileManager)->get_file(filepath).processed_contents();
			context.location = null_location(filepath);
		});
	}

	ScannerType Lexer::stype(void) const noexcept
	{
		return ScannerType::LEXER;
	}

	std::vector<Token> Lexer::lex()
	{
		std::vector<Token> tokens{};

		while (!eof())
		{
			using enum TokenType;

			// NOTE: intentionally making a copy here
			const auto here = current();
			const auto there = lookahead();

			if (my_isspace(here))
			{
				advance();
			}

			else if (my_isdigit(here))
			{
				const auto rest = substring_while(my_isdigit);
				APPEND_TOKEN(INT, rest);

				advance(rest.length()); // number
			}

			else if (here == '/')
			{
				if (there == '/')
				{
					// comment, no need to add a token
					skip_while([](auto c) { return c != '\n'; });
					advance(); // newline
					continue;
				}

				APPEND_TOKEN(SLASH, { here });

				advance(); // /
			}

			// NOTE: preprocessor will have already tried to expand macros
			// hexadecimal integer literal
			else if (here == '$')
			{
				advance(); // dollar sign
				
				const auto lexeme = substring_while(my_ishex);

				undo(); // rewind to dollar sign

				std::int32_t number{};
				auto [ptr, ec] = std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), number, 16);

				if (ec != std::errc())
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"unparseable integer literal `{}`", lexeme), forge_token(lexeme));
					advance(); // dollar sign
					advance(lexeme.length()); // number
					continue;
				}

				// convert back to a string to apply regional formatting for error reporting
				APPEND_TOKEN(INT, std::format("{}", number));

				advance(); // dollar sign
				advance(lexeme.length()); // number
			}

			else if (here == '"')
			{
				advance(); // opening quote

				// NOTE: does not allow for \" escape sequences!
				const auto string_literal = substring_while([](auto c) { return c != '"'; });

				undo(); // rewind to opening quote
				APPEND_TOKEN(STRING, string_literal);

				advance(); // opening quote
				advance(); // closing quote
			}

			else if (here == '.')
			{
				const auto lexeme = substring_while(my_isalnum);
				const auto search = _token_map.at(std::format(".{}", lexeme));

				if (search)
				{
					APPEND_TOKEN(*search, lexeme);
				}

				else
				{
					APPEND_TOKEN(IDENTIFIER, lexeme);
				}

				advance(); // .
				advance(lexeme.length()); // directive
			}

			else if (my_isidentifierfirst(here))
			{
				const auto lexeme = read_identifier();
				const auto search = _token_map.at(lexeme);

				if (search.has_value())
				{
					APPEND_TOKEN(search.value(), lexeme);
				}

				else
				{
					APPEND_TOKEN(IDENTIFIER, lexeme);
				}

				advance(lexeme.length());
			}

			else if (here == '!')
			{
				if (there == '=')
				{
					const auto lexeme = "!=";
					APPEND_TOKEN(EXCLAMATIONEQUALS, lexeme);
					advance(2); // !=
				}

				else
				{
					const auto lexeme = "!";
					APPEND_TOKEN(EXCLAMATION, lexeme);
					advance(); /// !
				}
			}

			else if (here == '=')
			{
				if (there == '=')
				{
					const auto lexeme = "==";
					APPEND_TOKEN(EQUALSEQUALS, lexeme);
					advance(2); // ==
				}

				else
				{
					const auto lexeme = "=";
					APPEND_TOKEN(EQUALS, lexeme);
					advance(); // =
				}
			}

			else
			{
				const auto current_string = std::string{ here };
				const auto search = _token_map.at(current_string);

				if (search.has_value())
				{
					APPEND_TOKEN(search.value(), current_string);
				}

				else
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"unexpected token `{}`", current_string), error_token(current_string));
				}

				advance(current_string.length());
			}
		}

		APPEND_TOKEN(TokenType::END, "eof");

		return tokens;
	}
}
