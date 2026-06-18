import std;

#include <toolchain/Scanner.h>
#include <io/FileManager.h>

// Haze Scanner.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	inline constexpr auto STRING_SCAN = "<inline source>";
}

namespace hz
{
	bool Scanner::eof() const
	{
		return current_context.eof();
	}

	char Scanner::current() const
	{
		return current_context.current();
	}

	char Scanner::lookahead() const
	{
		return current_context.lookahead();
	}

	std::size_t Scanner::whereat() const
	{
		return current_context.whereat();
	}

	std::filesystem::path Scanner::wherein() const
	{
		return current_context.wherein();
	}

	void Scanner::insert_adjacent(const std::string& source)
	{
		current_context.source.insert(whereat(), source);
		save_state();
	}

	void Scanner::advance(std::size_t count)
	{
		for (auto i = 0uz; i < count; ++i)
		{
			// NOTE: this will not correctly handle Windows \r\n
			if (current() == '\n')
			{
				current_context.location.line++;
				current_context.location.column = 1;
			}
			else
			{
				current_context.location.column++;
			}
			// although line and column updates vary by input, the running index always increments
			current_context.location.position++;
		}
		
		save_state();
	}

	bool Scanner::expect(char c, bool consume_whitespace)
	{
		if (auto actual = current(); actual != c)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format(
				"expected token `{}` but got `{}`", actual, c), forge_token());
			return false;
		}

		advance(1);

		if (consume_whitespace)
		{
			skip_whitespace(false);
		}

		save_state();
		return true;
	}

	bool Scanner::anticipate(char c, bool consume_whitespace)
	{
		auto try_consume = [&]
		{
			if (consume_whitespace)
			{
				skip_whitespace(false);
			}
		};

		if (current() == c)
		{
			advance(1);
			try_consume();
			return true;
		}

		try_consume();
		return false;
	}

	Token Scanner::forge_token(std::string_view text) const
	{
		// default to identifier unless the search proves otherwise
		auto kind = TokenKind::IDENTIFIER;
		if (token_map.contains(text))
		{
			kind = token_map.at(text).value();
		}

		return Token
		{
			.text = text,
			.location = current_context.location,
			.kind = kind,
			.confidence = 0,
		};
	}

	Token Scanner::forge_token() const
	{
		return forge_token(std::string_view(&current_context.source[whereat()], 1));
	}

	Token Scanner::error_token(std::string_view value)
	{
		return Token
		{
			.text = value,
			.location = current_context.location,
			.kind = TokenKind::ERROR,
		};
	}

	std::string_view Scanner::read_identifier(bool advance_context)
	{
		auto& content = current_context.source;

		const auto start = whereat();
		auto position = start;

		// special case for the first character
		if (position >= content.size() || !my_isidentifierfirst(content[position]))
		{
			return "";
		}

		position++;
		while (position < content.size() && my_isidentifier(content[position]))
		{
			position++;
		}

		const auto end = position;
		const auto length = end - start;

		if (advance_context)
		{
			advance(length);
		}

		// enforce URVO
		return content.substr(start, length);
	}

	bool Scanner::match_keyword(std::string_view keyword)
	{
		const auto length = keyword.length();

		if (current_context.source.compare(whereat(), length, keyword) == 0 &&
			(std::isspace(current_context.source[whereat() + length]) || current_context.source[whereat() + length] == '"'))
		{
			return true;
		}
		return false;
	}

	void Scanner::skip_whitespace(bool is_required)
	{
		auto count = 0uz;
		for (auto c = current(); std::isspace(c); count++)
		{
			// this is a weird for... loop :D
			c = current();
		}

		if (is_required && count == 0)
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"expected one or more whitespace tokens but got `{}`", current()), forge_token());
		}

		advance(count);
	}

	void Scanner::skip_until(char c)
	{
		while (!eof() && current() != c)
		{
			advance();
		}
	}

	void Scanner::skip_while(bool(*functor)(char))
	{
		while (!eof() && functor(current()))
		{
			advance();
		}
	}

	std::string_view Scanner::substring_until(char c, bool advance_until)
	{
		const auto source_length = current_context.source.length();
		const auto start = whereat();

		auto position = start;
		while (position < source_length && current_context.source[position] != c)
		{
			position++;
		}

		const auto length = position - start;
		if (advance_until)
		{
			advance(length);
		}

		return std::string_view{ current_context.source }.substr(start, length);
	}


	Scanner::Scanner(const std::filesystem::path& filepath)
		: current_context{ .source = USE_SAFE(FileManager)->get_file(filepath).get_raw_contents(), .location = null_location(filepath) }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "scanning");
	}

	Scanner::Scanner(std::string source)
		: current_context{ .source = std::move(source), .location = null_location(STRING_SCAN) }
	{
		USE_SAFE(ErrorReporter)->open_context(STRING_SCAN, "scanning");
	}

	Scanner::~Scanner()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}
}
