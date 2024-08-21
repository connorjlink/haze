#ifndef HAZE_EXPRESSIONTYPE_H
#define HAZE_EXPRESSIONTYPE_H

#define AS_EXPRESSION(x) static_cast<Expression*>(x)

#define AS_INTEGER_LITERAL_EXPRESSION(x) static_cast<IntegerLiteralExpression*>(x)
#define AS_IDENTIFIER_EXPRESSION(x) static_cast<IdentifierExpression*>(x)
#define AS_FUNCTION_CALL_EXPRESSION(x) static_cast<FunctionCallExpression*>(x)
#define AS_BINARY_EXPRESSION(x) static_cast<BinaryExpression*>(x)

namespace hz
{
	enum class ExpressionType
	{
		INTEGER_LITERAL,
		IDENTIFIER,
		FUNCTION_CALL,
		BINARY,
		ADJUST,
	};
}

#endif
