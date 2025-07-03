#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

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

		PERCENT,

		PIPE,

		CARET,

		LPAREN, RPAREN,
		LBRACE, RBRACE,
		LBRACKET, RBRACKET,

		PLUS, MINUS, STAR, SLASH,

		EQUALSEQUALS, EXCLAMATIONEQUALS,

		GREATER, LESS,

		LESSLESS, GREATERGREATER,

		TILDE, EXCLAMATION,

		FUNCTION,

		IMMUTABLE,
		MUTABLE,

		UNSIGNED,
		SIGNED,

		VALUE,
		PTR,

		BYTE,
		WORD,
		DWORD,
		QWORD,
		STRING,
		STRUCT,
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

		END, ERROR,

		DECLARE,

		U8, U16, U32, U64,
		S8, S16, S32, S64,
	};

#define S(x) std::string_view{ x }

	static const Bimap<std::string_view, TokenType> _token_map
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
		
		bimap_t{ S("immutable"), TokenType::IMMUTABLE },
		bimap_t{ S("mutable"), TokenType::MUTABLE },

		bimap_t{ S("unsigned"), TokenType::UNSIGNED },
		bimap_t{ S("signed"), TokenType::SIGNED },

		bimap_t{ S("value"), TokenType::VALUE },
		bimap_t{ S("ptr"), TokenType::PTR },

		bimap_t{ S("struct"), TokenType::STRUCT },

		bimap_t{ S("byte"), TokenType::BYTE },
		bimap_t{ S("word"), TokenType::WORD },
		bimap_t{ S("dword"), TokenType::DWORD },
		bimap_t{ S("qword"), TokenType::QWORD },

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

		bimap_t{ S("string"), TokenType::STRING },
		bimap_t{ S("[identifier]"), TokenType::IDENTIFIER },
		
		bimap_t{ S("declare"), TokenType::DECLARE },

		bimap_t{ S("eof"), TokenType::END },

		bimap_t{ S("ub"), TokenType::U8 },
		bimap_t{ S("uw"), TokenType::U16 },
		bimap_t{ S("ul"), TokenType::U32 },
		bimap_t{ S("uq"), TokenType::U64 },

		bimap_t{ S("sb"), TokenType::S8 },
		bimap_t{ S("sw"), TokenType::S16 },
		bimap_t{ S("sl"), TokenType::S32 },
		bimap_t{ S("sq"), TokenType::S64 },
	};

	struct SourceLocation
	{
		std::string filepath;
		// quick character index into the file
		std::size_t position;
		std::int32_t line, column;
	};

	struct Token
	{
		TokenType type;
		std::string text;
		SourceLocation location;
	};

	// NOTE: intentionally not marked constexpr to avoid errors about dropping qualifiers
	// could make certain inward-facing Token fields mutable, but this feels cleaner
	inline static const auto NULL_TOKEN = Token{ TokenType::END, "", { "unknown", 0, -1, -1 } };
	inline SourceLocation null_location(const std::string& filepath)
	{
		// specified file (optional) at (1, 1)
		return SourceLocation{ filepath, 0, 1, 1 };
	}
}

#endif
