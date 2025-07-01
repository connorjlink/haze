import std;

#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Generator.h"

// Haze IntegerLiteralExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType IntegerLiteralExpression::etype() const
	{
		return ExpressionType::INTEGER_LITERAL;
	}

	IntegerLiteralExpression* IntegerLiteralExpression::copy() const
	{
		return new IntegerLiteralExpression{ *this };
	}

	void IntegerLiteralExpression::generate(Allocation* allocation)
	{
		REQUIRE_SAFE(Generator)->make_immediate(allocation->read(), value);
	}

	Expression* IntegerLiteralExpression::optimize()
	{
		return nullptr;
	}

	Node* IntegerLiteralExpression::evaluate(Context* context) const
	{
		return copy();
	}
}
