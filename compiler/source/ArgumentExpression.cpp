import std;

#include <ast/expression/Expression.h>
#include <type/Type.h>

// Haze ArgumentExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType ArgumentExpression::etype() const
	{
		return ExpressionType::ARGUMENT;
	}

	TypeKind ArgumentExpression::ttype() const
	{
		return type->ttype();
	}

	void ArgumentExpression::generate(Allocation*)
	{
		USE_SAFE(ErrorReporter)->post_error("unsupported compiler expression type `argument`", _token);
	}

	Expression* ArgumentExpression::optimize()
	{
		// no optimizations possible for an argument expression
		return nullptr;
	}

	Node* ArgumentExpression::evaluate(Context* context) const
	{
		return identifier->copy();
	}
}