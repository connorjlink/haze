#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <string>
#include <cstdint>

#include "Bimap.h"

// Haze Token.h
// (c) Connor J. Link. All Rights Reserved.

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

		EQUALSEQUALS, EXCLAMATIONEQUALS,

		GREATER, LESS,

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

		MOVE, LOAD, SAVE, COPY, IADD, ISUB, BAND, BIOR, BXOR, CALL, EXIT, PUSH, PULL, BRNZ, BOOL, STOP,

		PRINT,
		STRING,

		END,
	};

#define S(x) std::string{ x }

	static const Bimap<std::string, TokenType> _token_map
	{
		bimap_t{ S(";"), TokenType::SEMICOLON },
		bimap_t{ S(":"), TokenType::COLON },
		bimap_t{ S(","), TokenType::COMMA },
		bimap_t{ S("="), TokenType::EQUALS },
		bimap_t{ S("&"), TokenType::AMPERSAND },
		bimap_t{ S("#"), TokenType::POUND },
		bimap_t{ S("("), TokenType::LPAREN  },
		bimap_t{ S(")"), TokenType::RPAREN },
		bimap_t{ S("{"), TokenType::LBRACE },
		bimap_t{ S("}"), TokenType::RBRACE },
		bimap_t{ S("["), TokenType::LBRACKET },
		bimap_t{ S("]"), TokenType::RBRACKET },
		bimap_t{ S("+"), TokenType::PLUS },
		bimap_t{ S("-"), TokenType::MINUS },
		bimap_t{ S("*"), TokenType::STAR },
		bimap_t{ S("=="), TokenType::EQUALSEQUALS },
		bimap_t{ S("!="), TokenType::EXCLAMATIONEQUALS },
		bimap_t{ S(">"), TokenType::GREATER },
		bimap_t{ S("<"), TokenType::LESS },
		bimap_t{ S("~"), TokenType::TILDE },
		bimap_t{ S("!"), TokenType::EXCLAMATION },
		
		bimap_t{ S("function"), TokenType::FUNCTION },
		bimap_t{ S("byte"), TokenType::BYTE },
		bimap_t{ S("nvr"), TokenType::NVR},
		bimap_t{ S("return"), TokenType::RETURN },
		bimap_t{ S("while"), TokenType::WHILE },
		bimap_t{ S("for"), TokenType::FOR },
		bimap_t{ S("if"), TokenType::IF },
		bimap_t{ S("else"), TokenType::ELSE },
		
		bimap_t{ S("intrinsic"), TokenType::INTRINSIC },
		bimap_t{ S("geometry"), TokenType::GEOMETRY },
		
		bimap_t{ S(".define"), TokenType::DOTDEFINE },
		bimap_t{ S(".org"), TokenType::DOTORG },
		
		bimap_t{ S(".hook"), TokenType::DOTHOOK },
		bimap_t{ S(".unhook"), TokenType::DOTUNHOOK },
		
		bimap_t{ S("asm"), TokenType::ASM },
		
		bimap_t{ S("r0"), TokenType::R0 },
		bimap_t{ S("r1"), TokenType::R1 },
		bimap_t{ S("r2"), TokenType::R2 },
		bimap_t{ S("r3"), TokenType::R3 },

		bimap_t{ S("move"), TokenType::MOVE },
		bimap_t{ S("load"), TokenType::LOAD },
		bimap_t{ S("copy"), TokenType::COPY },
		bimap_t{ S("iadd"), TokenType::IADD },
		bimap_t{ S("isub"), TokenType::ISUB },
		bimap_t{ S("band"), TokenType::BAND },
		bimap_t{ S("bior"), TokenType::BIOR },
		bimap_t{ S("bxor"), TokenType::BXOR },
		bimap_t{ S("call"), TokenType::CALL },
		bimap_t{ S("exit"), TokenType::EXIT },
		bimap_t{ S("push"), TokenType::PUSH },
		bimap_t{ S("pull"), TokenType::PULL },
		bimap_t{ S("brnz"), TokenType::BRNZ },
		bimap_t{ S("stop"), TokenType::STOP },
		
		bimap_t{ S("print"), TokenType::PRINT },
		bimap_t{ S("[string literal]"), TokenType::STRING },
		
		bimap_t{ S("eof"), TokenType::END },
	};

	struct Token
	{
		TokenType type;
		std::string value;
		std::int16_t line, column;
	};

	static auto NULL_TOKEN = Token{ TokenType::END, "", -1, -1 };
}

#endif
