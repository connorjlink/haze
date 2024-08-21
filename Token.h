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
		COLON,
		COMMA,
		EQUALS,
		AMPERSAND,
		POUND,

		LPAREN, RPAREN,
		LBRACE, RBRACE,
		LBRACKET, RBRACKET,

		PLUS, MINUS, STAR,

		TILDE, EXCLAMATION,

		FUNCTION,
		BYTE,
		NVR,
		RETURN,
		WHILE,
		FOR,
		IF,
		ELSE,

		INTRINSIC,
		GEOMETRY,

		DOTDEFINE, DOTORG,
		DOTHOOK, DOTUNHOOK,

		ASM,
		R0, R1, R2, R3,

		MOVE, LOAD, SAVE, COPY, IADD, ISUB, BAND, BIOR, BXOR, BNOT, CALL, EXIT, PUSH, PULL, BREZ, WAIT,

		PRINT,
		STRING,

		END,
	};

	static const std::unordered_map<std::string_view, TokenType> lexeme_map =
	{
		{ ";", TokenType::SEMICOLON },
		{ ":", TokenType::COLON },
		{ ",", TokenType::COMMA },
		{ "=", TokenType::EQUALS },
		{ "&", TokenType::AMPERSAND },
		{ "#", TokenType::POUND },
		{ "(", TokenType::LPAREN  },
		{ ")", TokenType::RPAREN },
		{ "{", TokenType::LBRACE },
		{ "}", TokenType::RBRACE },
		{ "[", TokenType::LBRACKET },
		{ "]", TokenType::RBRACKET },
		{ "+", TokenType::PLUS },
		{ "-", TokenType::MINUS },
		{ "*", TokenType::STAR },
		{ "~", TokenType::TILDE },
		{ "!", TokenType::EXCLAMATION },

		{ "function", TokenType::FUNCTION },
		{ "byte", TokenType::BYTE },
		{ "nvr", TokenType::NVR},
		{ "return", TokenType::RETURN },
		{ "while", TokenType::WHILE },
		{ "for", TokenType::FOR },
		{ "if", TokenType::IF },
		{ "else", TokenType::ELSE },

		{ "intrinsic", TokenType::INTRINSIC },
		{ "geometry", TokenType::GEOMETRY },

		{ ".define", TokenType::DOTDEFINE },
		{ ".org", TokenType::DOTORG },

		{ ".hook", TokenType::DOTHOOK },
		{ ".unhook", TokenType::DOTUNHOOK },

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

		{ "print", TokenType::PRINT },
		{ "[string literal]", TokenType::STRING },

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
