#ifndef HAZE_BINARYEXPRESSIONTYPE_H
#define HAZE_BINARYEXPRESSIONTYPE_H

#include "Token.h"

// Haze BinaryExpressionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class BinaryExpressionType
	{
		PLUS,
		MINUS,
		TIMES,

		OR,
		XOR,
		AND,

		LSHIFT,
		RSHIFT,

		ASSIGN,

		EQUALITY,
		INEQUALITY,

		GREATER,
		LESS,
	};

	static const std::unordered_map<TokenType, BinaryExpressionType> _binary_expression_map
	{
		{ TokenType::PLUS,              BinaryExpressionType::PLUS },
		{ TokenType::MINUS,             BinaryExpressionType::MINUS },
		{ TokenType::STAR,              BinaryExpressionType::TIMES },
		{ TokenType::EQUALS,            BinaryExpressionType::ASSIGN },
		{ TokenType::EQUALSEQUALS,      BinaryExpressionType::EQUALITY },
		{ TokenType::EXCLAMATIONEQUALS, BinaryExpressionType::INEQUALITY },
		{ TokenType::GREATER,           BinaryExpressionType::GREATER },
		{ TokenType::LESS,              BinaryExpressionType::LESS },
	};
}

#endif
