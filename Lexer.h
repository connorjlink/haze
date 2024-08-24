#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include <string>
#include <vector>

#include "Token.h"

namespace hz
{
	class Lexer
	{
	private:
		std::string input;
		std::int32_t line = 1;

	public:
		Lexer(std::string&& input)
			: input{ std::move(input) }
		{
		}

	public:
		std::vector<Token> lex();
	};
}

#endif //HAZE_LEXER_H
