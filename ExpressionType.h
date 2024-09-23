#ifndef HAZE_EXPRESSIONTYPE_H
#define HAZE_EXPRESSIONTYPE_H

// Haze ExpressionType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_EXPRESSION(x) static_cast<Expression*>(x)

#define AS_INTEGER_LITERAL_EXPRESSION(x) static_cast<IntegerLiteralExpression*>(x)
#define AS_IDENTIFIER_EXPRESSION(x) static_cast<IdentifierExpression*>(x)
#define AS_FUNCTION_CALL_EXPRESSION(x) static_cast<FunctionCallExpression*>(x)
#define AS_BINARY_EXPRESSION(x) static_cast<BinaryExpression*>(x)
#define AS_ADJUST_EXPRESSION(x) static_cast<AdjustExpression*>(x)
#define AS_STRING_EXPRESSION(x) static_cast<StringExpression*>(x)
#define AS_ARGUMENT_EXPRESSION(x) static_cast<ArgumentExpression*>(x)

namespace hz
{
	enum class ExpressionType
	{
		INTEGER_LITERAL,
		IDENTIFIER,
		FUNCTION_CALL,
		BINARY,
		ADJUST,
		STRING,
		ARGUMENT,
		FUNCTION_ARGUMENT, // NOTE: is similar to argument, but used when passing arguments 
						   // back to the code generator for function calls. Adds an extra
						   // "parameter" Expression* to represent the value passed
	};

	static const std::unordered_map<ExpressionType, std::string_view> _expression_type_map
	{
		{ ExpressionType::INTEGER_LITERAL, "integer literal" },
		{ ExpressionType::IDENTIFIER, "identifier" },
		{ ExpressionType::FUNCTION_CALL, "function call" },
		{ ExpressionType::BINARY, "binary" },
		{ ExpressionType::ADJUST, "adjustment" },
		{ ExpressionType::STRING, "string" },
		{ ExpressionType::ARGUMENT, "argument" },
	};
}

#endif
