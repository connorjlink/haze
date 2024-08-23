#ifndef HAZE_BINARYEXPRESSIONTYPE_H
#define HAZE_BINARYEXPRESSIONTYPE_H

namespace hz
{
	enum class BinaryExpressionType
	{
		PLUS,
		MINUS,
		TIMES,

		ASSIGN,

		EQUALITY,
		INEQUALITY,

		GREATER,
		LESS,
	};
}

#endif
