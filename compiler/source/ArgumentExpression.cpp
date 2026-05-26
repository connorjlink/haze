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

	TypeKind ArgumentExpression::tag_type() const
	{
		return type->tag_type();
	}

	void ArgumentExpression::generate(ValueHandle)
	{
		USE_SAFE(ErrorReporter)->post_error("unsupported compiler expression type `argument`", _token);
	}

	ExpressionHandle ArgumentExpression::optimize()
	{
		// no optimizations possible for an argument expression
		return nullptr;
	}

	Node* ArgumentExpression::evaluate(Context* context) const
	{
		return identifier->copy();
	}
}