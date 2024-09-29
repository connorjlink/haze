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
		std::int32_t _character;

	private:
		std::int16_t _line, _column;

	public:
		Lexer(const std::string& input)
			: _input{ input }, _character{ -1 }
		{
			// NOTE: line and column default value is 1, not 0 for better debug messaging
			_line = 1;
			_column = 1;
		}

	public:
		std::vector<Token> lex();
	};
}

#endif
