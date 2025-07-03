import std;

#include "Scanner.h"
#include "FileManager.h"

// Haze Scanner.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Scanner::Scanner(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "scanning");
	}

	Scanner::~Scanner()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}

	bool Scanner::eof(void) const
	{
		return _current_context.eof();
	}

	char Scanner::current(void) const
	{
		return _current_context.current();
	}

	char Scanner::lookahead(void) const
	{
		return _current_context.lookahead();
	}

	std::size_t Scanner::whereat(void) const
	{
		return _current_context.whereat();
	}

	std::string Scanner::wherein(void) const
	{
		return _current_context.wherein();
	}

	void Scanner::insert_adjacent(const std::string& source)
	{
		_current_context.source.insert(whereat(), source);
		save_state();
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
			USE_UNSAFE(ErrorReporter)->post_error(std::format(
				"expected token `{}` to specify an include filepath but got `{}`", actual, c), forge_token());
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

	Token Scanner::forge_token(const std::string& text) const
	{
		// default to identifier unless the search proves otherwise
		auto type = TokenType::IDENTIFIER;
		if (_token_map.contains(text))
		{
			// has_value() strengthened
			type = _token_map.at(text).value();
		}

		return Token
		{
			.type = type,
			.text = text,
			.location = _current_context.location,
		};
	}

	Token Scanner::forge_token(void) const
	{
		return forge_token({ current() });
	}

	Token Scanner::error_token(const std::string& value)
	{
		Token token{};

		token.type = TokenType::ERROR;
		token.text = value;
		token.location = _current_context.location;

		return token;
	}

	std::string Scanner::read_identifier(bool advance_context)
	{
		auto& content = _current_context.source;

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

		if (_current_context.source.compare(whereat(), length, keyword) == 0 &&
			(std::isspace(_current_context.source[whereat() + length]) || _current_context.source[whereat() + length] == '"'))
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
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"expected one or more whitespace tokens but got `{}`", current()), forge_token());
		}

		advance(number_to_skip);
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

	std::string Scanner::substring_until(char c, bool advance_until)
	{
		const auto source_length = _current_context.source.length();
		const auto start = whereat();

		auto position = start;
		while (position < source_length && _current_context.source[position] != c)
		{
			position++;
		}

		const auto length = position - start;
		if (advance_until)
		{
			advance(length);
		}

		// enforce URVO
		return _current_context.source.substr(start, length);
	}

	std::string Scanner::substring_while(bool(*functor)(char), bool advance_while)
	{
		const auto source_length = _current_context.source.length();
		const auto start = whereat();

		auto position = start;
		while (position < source_length && functor(_current_context.source[position]))
		{
			position++;
		}

		const auto length = position - start;
		if (advance_while)
		{
			advance(length);
		}

		// enforce URVO
		return _current_context.source.substr(start, length);
	}
}
