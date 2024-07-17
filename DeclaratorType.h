#ifndef HAZE_DECLARATORTYPE_H
#define HAZE_DECLARATORTYPE_H

#define AS_DECLARATOR(x) static_cast<Declarator*>(x)

#define AS_INTRINSIC_DECLARATOR(x) static_cast<IntrinsicDeclarator*>(x)
#define AS_FUNCTION_DECLARATOR(x) static_cast<FunctionDeclarator*>(x)
#define AS_STATEMENT_DECLARATOR(x) static_cast<StatementDeclarator*>(x)
#define AS_EXPRESSION_DECLARATOR(x) static_cast<ExpressionDeclarator*>(x)

namespace hz
{
	enum class Type
	{
		INTRINSIC,
		FUNCTION,
		STATEMENT,
		EXPRESSION,
	};
}

#endif 
