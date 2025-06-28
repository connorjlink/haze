#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include "Token.h"

// Haze Lexer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Lexer
	{
	private:
		const std::string& _input;
		const std::string& _filepath;

	private:
		// position tracking information
		std::size_t _position;
		std::int32_t _line, _column;

	public:
		Lexer(const std::string& input, const std::string& filepath)
			: _input{ input }, _filepath{ filepath }
		{
			_position = 0;
			// NOTE: line and column default value is 1, not 0 for better debug messaging
			_line = 1;
			_column = 1;
		}

	public:
		void advance(std::size_t = 0);
		void expect(char);
		void expect(const std::string&);
		std::vector<Token> lex();
	};
}

#endif
