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

		LPAREN, RPAREN,
		LBRACE, RBRACE,

		PLUS, MINUS, STAR,

		FUNCTION,
		BYTE,
		RETURN,

		DOTDEFINE, DOTORG,

		MOVE, LOAD, SAVE, COPY, IADD, ISUB, BAND, BIOR, BXOR, BNOT, CALL, EXIT, PUSH, PULL, BREZ, WAIT,

		END,
	};

	static const std::unordered_map<std::string_view, TokenType> lexeme_map =
	{
		{ ";", TokenType::SEMICOLON },
		{ ",", TokenType::COMMA },
		{ "=", TokenType::EQUALS },
		{ "(", TokenType::LPAREN  },
		{ ")", TokenType::RPAREN },
		{ "{", TokenType::LBRACE },
		{ "}",TokenType::RBRACE },
		{ "+", TokenType::PLUS },
		{ "-", TokenType::MINUS },
		{ "*", TokenType::STAR },

		{ "function", TokenType::FUNCTION },
		{ "byte", TokenType::BYTE },
		{ "return", TokenType::RETURN },

		{ ".define", TokenType::DOTDEFINE },
		{ ".org", TokenType::DOTORG },

		{ "move", TokenType::MOVE },
		{ "load", TokenType::LOAD },
		{ "copy", TokenType::COPY },
		{ "iadd", TokenType::IADD },
		{ "isub", TokenType::ISUB },
		{ "BAND", TokenType::BAND },
		{ "BIOR", TokenType::BIOR },
		{ "BXOR", TokenType::BXOR },
		{ "BNOT", TokenType::BNOT },
		{ "CALL", TokenType::CALL },
		{ "EXIT", TokenType::EXIT },
		{ "PUSH", TokenType::PUSH },
		{ "PULL", TokenType::PULL },
		{ "BREZ", TokenType::BREZ },
		{ "WAIT", TokenType::WAIT },

		{ "eof", TokenType::END },
	};

	struct Token
	{
		TokenType type;
		std::size_t line;
		std::optional<std::string> value;
	};
}

#endif //HAZE_TOKEN_H
