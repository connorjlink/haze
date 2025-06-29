import std;

#include "Scanner.h"
#include "FileManager.h"

// Haze Scanner.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Scanner::Scanner(const std::string& filepath)
	{
#error TODO CONSTRUCT WITH SOMETHING OTHER THAN RAW CONTENTS!!!!! THIS WILL NOT WORK FOR THE LEXER
		const auto& contents = USE_SAFE(FileManager).get_file(filepath).raw_contents();
		set_state(SourceContext{ .source = contents, .location = null_location(filepath) });
		save_state();
	}

	char Scanner::current(void) const
	{
		if (!eof())
		{
			return _current_context.source[where()];
		}
		return '\0';
	}

	std::size_t Scanner::where(void) const
	{
		return _current_context.location.position;
	}

	bool Scanner::eof(void) const
	{
		return where() >= _current_context.source.length();
	}

	void Scanner::insert_adjacent(const std::string& source)
	{
		_current_context.source.insert(where(), source);
	}

	void Scanner::advance(std::size_t how_many)
	{
		for (int i = 0u; i < how_many; ++i)
		{
			// NOTE: this will not correctly handle Windows \r\n
			if (current() == '\n')
			{
				_current_context.location.line++;
				_current_context.location.column = 1;
			}
			else
			{
				_current_context.location.column++;
			}
			// although line and column updates vary by input, the running index always increments
			_current_context.location.position++;
		}
		
		save_state();
	}

	bool Scanner::expect(char c, bool consume_whitespace)
	{
		if (auto actual = current(); actual != c)
		{
			USE_UNSAFE(ErrorReporter).post_error(std::format(
				"expected token `{}` to specify an include filepath but got `{}`", actual, c), forge_token());
			return false;
		}

		advance(1);

		if (consume_whitespace)
		{
			skip_whitespace(false);
		}
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

	Token Scanner::forge_token(void) const
	{
		auto character = std::string{ current() };

		// default to identifier unless the search proves otherwise
		auto type = TokenType::IDENTIFIER;
		if (_token_map.contains(character))
		{
			// has_value() strengthened
			type = _token_map.at(character).value();
		}

		return Token
		{
			.type = type,
			.text = character,
			.location = _current_context.location,
		};
	}

	std::string Scanner::read_identifier(bool advance_context)
	{
		auto& content = _current_context.source;

		const auto start = where();
		auto position = start;

		// special case for the first character
		if (position >= content.size() || !is_identifier_first(content[position]))
		{
			return "";
		}

		position++;
		while (position < content.size() && is_identifier(content[position]))
		{
			position++;
		}

		const auto end = position;
		const auto length = end - start;

		if (advance_context)
		{
			advance(length);
		}

		return content.substr(start, length);
	}

	bool Scanner::match_keyword(std::string_view keyword)
	{
		const auto length = keyword.length();

		if (_current_context.source.compare(where(), length, keyword) == 0 &&
			(std::isspace(_current_context.source[where() + length]) || _current_context.source[where() + length] == '"'))
		{
			return true;
		}
		return false;
	}

	void Scanner::skip_whitespace(bool is_required)
	{
		int number_to_skip = 0;
		for (auto c = current(); std::isspace(c); number_to_skip++)
		{
			// this is a weird for... loop :D
			c = current();
		}

		if (is_required && number_to_skip == 0)
		{
			USE_SAFE(ErrorReporter).post_error(std::format(
				"expected one or more whitespace tokens but got `{}`", current()), forge_token());
		}

		advance(number_to_skip);
	}

	std::string Scanner::substring_until(char c, bool advance_until)
	{
		const auto start = where();

		auto position = start;
		while (current() != '"' && !eof())
		{
			position++;
		}

		const auto length = position - start;

		if (advance_until)
		{
			advance(length);
		}

		const auto string = _current_context.source.substr(start, length);
		return string;
	}
}
