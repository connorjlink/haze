import std;

#include <toolchain/Lexer.h>
#include <utility/Constants.h>

// Haze Lexer.cpp
// (c) Connor J. Link. All Rights Reserved.

#define APPEND_TOKEN(kind, raw) tokens.emplace_back(raw, get_context().location, kind, 0)

namespace
{
	bool is_newline(char c)
	{
		return c == '\n' || c == '\r';
	}
}

namespace hz
{
	std::string_view Lexer::read_integer_literal_suffix()
	{
		// NOTE: mixing capitalization is not valid in C99
		static const std::unordered_set<std::string_view> suffixes
		{
			"u", "U", "l", "L", "ul", "UL", "lu", "LU",
			"ull", "ULL", "llu", "LLU",
			"f", "F", "l", "L"
		};

		const auto candidate = substring_while([](auto c) { return my_isalpha(c); });

		if (!suffixes.contains(candidate))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid integer literal suffix `{}`", candidate), forge_token());
			return std::string_view{};
		}

		return candidate;
	}

	std::string_view Lexer::read_float_literal_suffix()
	{
		static const std::unordered_set<std::string_view> suffixes
		{
			"f", "F", "l", "L"
		};

		const auto candidate = substring_while([](auto c) { return my_isalpha(c); });
		
		if (!suffixes.contains(candidate))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid float literal suffix `{}`", candidate), forge_token());
			return std::string_view{};
		}

		return candidate;
	}

	Token Lexer::expect_identifier()
	{
		if (!my_isidentifierfirst(current()))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("expected identifier, got `{}`", current()), forge_token());
			return error_token("expected identifier");
		}

		const auto identifier = read_identifier();
		return Token
		{
			.text = identifier,
			.location = get_context().location,
			.kind = TokenKind::IDENTIFIER,
		};
	}

	Token Lexer::expect_integer_literal()
	{
		if (!my_isdigit(current()))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("expected integer literal, got `{}`", current()), forge_token());
			return error_token("expected integer literal");
		}

		auto integer_literal = std::string_view{};

		switch (lookahead())
		{
			case 'x':
			{
				// hexadecimal
				advance(2);
				integer_literal = substring_while(my_ishex);

			} break;

			case 'o':
			{
				// octal
				advance(2);
				integer_literal = substring_while(my_isoctal);
			} break;

			case 'b':
			{
				// binary
				advance(2);
				integer_literal = substring_while(my_isbinary);
			} break;

			default:
			{
				// decimal
				integer_literal = substring_while(my_isdigit);
			} break;
		}

		const auto suffix = read_integer_literal_suffix();
		integer_literal = std::string_view{ integer_literal.data(), integer_literal.size() + suffix.size() };

		return Token
		{
			.text = integer_literal,
			.location = get_context().location,
			.kind = TokenKind::INTEGER_LITERAL,
		};
	}

	Token Lexer::expect_float_literal()
	{
		const auto start = &get_context().source[get_context().whereat()];
		auto length = 0uz;

		if (!my_isdigit(current()) && current() != '.')
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("expected float literal, got `{}`", current()), forge_token());
			return error_token("expected float literal");
		}

		auto is_hex = false;
		if (current() == '0' && (lookahead() == 'x' || lookahead() == 'X'))
		{
			is_hex = true;
			advance(2);
			length += 2;
		}

		auto is_valid_digit = [&](char c)
		{
			return is_hex ? my_ishex(c) : my_isdigit(c);
		};

		auto count = 0uz;

		while (is_valid_digit(current()))
		{
			advance();
			length++;
			count++;
		}

		if (current() == '.')
		{
			advance();
			length++;

			while (is_valid_digit(current()))
			{
				advance();
				length++;
				count++;
			}
		}

		if (count == 0)
		{
			USE_SAFE(ErrorReporter)->post_error("float literal must contain at least one digit", forge_token());
			return error_token("invalid float literal");
		}

		const auto exp_lower = is_hex ? 'p' : 'e';
		const auto exp_upper = is_hex ? 'P' : 'E';

		if (current() != exp_lower && current() != exp_upper && is_hex)
		{
			USE_SAFE(ErrorReporter)->post_error("hexadecimal floating constants require an exponent", forge_token());
			return error_token("missing exponent in hex float");
		}

		if (current() == exp_lower || current() == exp_upper)
		{
			advance();
			length++;

			if (current() == '+' || current() == '-')
			{
				advance();
				length++;
			}

			if (!my_isdigit(current()))
			{
				USE_SAFE(ErrorReporter)->post_error(std::format("expected exponent digits but got `{}`", current()), forge_token());
				return error_token("expected exponent digits");
			}

			while (my_isdigit(current()))
			{
				advance();
				length++;
			}
		}

		const auto suffix = read_float_literal_suffix();
		length += suffix.size();

		return Token
		{
			.text = std::string_view{ start, length },
			.location = get_context().location,
			.kind = TokenKind::FLOAT_LITERAL,
		};
	}

	Token Lexer::expect_string_literal()
	{
		expect('"');

		// will internally handle \" escape sequences
		const auto string_literal = substring_while([](auto c) { return c != '"' && !is_newline(c); });

		expect('"');

		return Token
		{
			.text = string_literal,
			.location = get_context().location,
			.kind = TokenKind::STRING_LITERAL,
		};
	}

	Token Lexer::expect_wide_string_literal()
	{
		expect('L');

		expect('"');
		// will internally handle \" escape sequences
		const auto string_literal = substring_while([](auto c) { return c != '"' && !is_newline(c); });
		expect('"');
		
		return Token
		{
			.text = string_literal,
			.location = get_context().location,
			.kind = TokenKind::WIDE_STRING_LITERAL,
		};
	}

	Token Lexer::expect_character_literal()
	{
		expect('\'');

		// will internally handle \' escape sequences
		const auto char_literal = substring_while([](auto c) { return c != '\'' && !is_newline(c); });

		expect('\'');

		return Token
		{
			.text = char_literal,
			.location = get_context().location,
			.kind = TokenKind::CHARACTER_LITERAL,
		};
	}


	ScannerKind Lexer::scanner_kind() const
	{
		return ScannerKind::LEXER;
	}

	std::vector<Token> Lexer::lex()
	{
		auto tokens = std::vector<Token>{};

#pragma message("TODO: finish generating every operator and keyword token with manual maximal munch")

		while (!eof())
		{
			using enum TokenKind;

			// NOTE: intentionally making a copy here
			const auto here = current();
			const auto there = lookahead();

			if (my_isspace(here))
			{
				advance();
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

				// C rules for commas in macro arguments requiring the variadic expand xD
				APPEND_TOKEN(SLASH, EXPAND(std::string_view{ &here, 1 }));

				advance();
			}
			else if (here == '"')
			{
				
			}
			else if (here == '\'')
			{
				advance(); // opening quote

				const auto char_literal = substring_while([](auto c) { return c != '\''; });

				undo(); // rewind to opening quote
				APPEND_TOKEN(CHARACTER_LITERAL, char_literal);

				advance(); // opening quote
				advance(); // closing quote
			}
			else if (my_isidentifierfirst(here))
			{
				const auto lexeme = read_identifier();
				const auto search = token_map.at(lexeme);

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
					advance(2);
				}
				else
				{
					const auto lexeme = "!";
					APPEND_TOKEN(EXCLAMATION, lexeme);
					advance();
				}
			}
			else if (here == '=')
			{
				if (there == '=')
				{
					const auto lexeme = "==";
					APPEND_TOKEN(EQUALSEQUALS, lexeme);
					advance(2);
				}
				else
				{
					const auto lexeme = "=";
					APPEND_TOKEN(EQUALS, lexeme);
					advance();
				}
			}
			else
			{
				const auto current_string = std::string{ here };
				const auto search = token_map.at(current_string);

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

		APPEND_TOKEN(TokenKind::END, "eof");

		return tokens;
	}


	Lexer::Lexer(const std::filesystem::path& filepath)
		: Scanner{ filepath }
	{
		// transact an initial state for undoing
		commit<void>([&](auto& context) -> void
		{
			context.source = USE_SAFE(FileManager)->get_file(filepath).get_processed_contents();
			context.location = null_location(filepath);
		});
	}

	Lexer::Lexer(std::string source)
		: Scanner{ std::move(source) }
	{
		// transact an initial state for undoing
		commit<void>([&](auto& context) -> void
		{
			context.source = std::move(source);
			context.location = null_location(STRING_SCAN);
		});
	}
}
