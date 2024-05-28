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
		NVR,
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
		std::optional<std::string> value;
	};
}

#endif //HAZE_TOKEN_H
