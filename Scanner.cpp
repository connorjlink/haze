import std;

#include "Scanner.h"
#include "FileManager.h"

// Haze Scanner.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Scanner::Scanner(const std::string& filepath)
	{
		const auto& contents = USE(FileManager).get_file(filepath).contents();
		set_state({ .source = contents, .location = null_location(filepath) });
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
	}

	void Scanner::advance(std::size_t how_many)
	{
	}

	void Scanner::expect(char)
	{
	}
	
	void Scanner::expect(const std::string&)
	{
	}

	bool Scanner::anticipate(char c, bool consume_whitespace)
	{
		auto try_consume = [&]
		{
			if (consume_whitespace)
			{
				// try consume as requested, ok if none
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

	bool Scanner::peek_match_identifier(void) const
	{
		return false;
	}

	void Scanner::skip_whitespace(bool is_required)
	{
		int number_to_skip = 0;
		for (auto c = current(); std::isspace(c); number_to_skip++)
		{
			// this is a weird for... loop :D
		}

		if (is_required && number_to_skip == 0)
		{
			USE(ErrorReporter).post_error(std::format(
				"expectd one or more whitespace tokens but got `{}`", current()), forge();
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
