#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include "Node.h"

#define AS_INTEGER_LITERAL_EXPRESSION(x) static_cast<IntegerLiteralExpression*>(x)
#define AS_IDENTIFIER_EXPRESSION(x) static_cast<IdentifierExpression*>(x)
#define AS_FUNCTION_CALL_EXPRESSION(x) static_cast<FunctionCallExpression*>(x)
#define AS_BINARY_EXPRESSION(x) static_cast<BinaryExpression*>(x)

namespace hz
{
	class Expression : public Node
	{
	public:
		enum class Type
		{
			INTEGER_LITERAL,
			IDENTIFIER,
			FUNCTION_CALL,
			BINARY,
		};

		//TODO: implement virtual destructor for this class
	public:
		virtual Node::Type ntype() const final override;
		virtual Expression::Type etype() const = 0;
		virtual Expression* optimize() override; //return nullptr--default is to do nothing
	};
}

#endif //HAZE_EXPRESSION_H
