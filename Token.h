#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <cstdlib>
#include <string>
#include <optional>
#include <unordered_map>

namespace hz
{
	enum class TokenType
	{
		INT,

		IDENTIFIER,

		SEMICOLON,
		COMMA,
		EQUALS,
		AMPERSAND,
		POUND,

		LPAREN, RPAREN,
		LBRACE, RBRACE,

		PLUS, MINUS, STAR,

		FUNCTION,
		BYTE,
		NVR,
		RETURN,

		DOTDEFINE, DOTORG,

		ASM,
		R0, R1, R2, R3,

		MOVE, LOAD, SAVE, COPY, IADD, ISUB, BAND, BIOR, BXOR, BNOT, CALL, EXIT, PUSH, PULL, BREZ, WAIT,

		END,
	};

	static const std::unordered_map<std::string_view, TokenType> lexeme_map =
	{
		{ ";", TokenType::SEMICOLON },
		{ ",", TokenType::COMMA },
		{ "=", TokenType::EQUALS },
		{ "&", TokenType::AMPERSAND },
		{ "#", TokenType::POUND },
		{ "(", TokenType::LPAREN  },
		{ ")", TokenType::RPAREN },
		{ "{", TokenType::LBRACE },
		{ "}", TokenType::RBRACE },
		{ "+", TokenType::PLUS },
		{ "-", TokenType::MINUS },
		{ "*", TokenType::STAR },

		{ "function", TokenType::FUNCTION },
		{ "byte", TokenType::BYTE },
		{ "nvr", TokenType::NVR},
		{ "return", TokenType::RETURN },

		{ ".define", TokenType::DOTDEFINE },
		{ ".org", TokenType::DOTORG },

		{ "asm", TokenType::ASM },

		{ "r0", TokenType::R0 },
		{ "r1", TokenType::R1 },
		{ "r2", TokenType::R2 },
		{ "r3", TokenType::R3 },

		{ "move", TokenType::MOVE },
		{ "load", TokenType::LOAD },
		{ "copy", TokenType::COPY },
		{ "iadd", TokenType::IADD },
		{ "isub", TokenType::ISUB },
		{ "band", TokenType::BAND },
		{ "bior", TokenType::BIOR },
		{ "bxor", TokenType::BXOR },
		{ "bnot", TokenType::BNOT },
		{ "call", TokenType::CALL },
		{ "exit", TokenType::EXIT },
		{ "push", TokenType::PUSH },
		{ "pull", TokenType::PULL },
		{ "brez", TokenType::BREZ },
		{ "wait", TokenType::WAIT },

		{ "eof", TokenType::END },
	};

	struct Token
	{
		TokenType type;
		std::size_t line;
		std::string value;
	};
}

#endif //HAZE_TOKEN_H
