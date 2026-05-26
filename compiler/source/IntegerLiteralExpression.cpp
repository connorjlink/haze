import std;

#include <allocator/Allocator.h>
#include <ast/expression/Expression.h>
#include <toolchain/Generator.h>

// Haze IntegerLiteralExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType IntegerLiteralExpression::etype() const
	{
		return ExpressionType::INTEGER_LITERAL;
	}

	TypeKind IntegerLiteralExpression::tag_type() const
	{
		return TypeKind::INT;
	}

	void IntegerLiteralExpression::generate(ValueHandle allocation)
	{
		REQUIRE_SAFE(Generator)->make_immediate(allocation->read(), value);
	}

	ExpressionHandle IntegerLiteralExpression::optimize()
	{
		return nullptr;
	}

	Node* IntegerLiteralExpression::evaluate(Context* context) const
	{
		return copy();
	}
}
