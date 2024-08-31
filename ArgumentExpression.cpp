#include "ArgumentExpression.h"

// Haze ArgumentExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType ArgumentExpression::etype() const
	{
		return ExpressionType::ARGUMENT;
	}

	ArgumentExpression* ArgumentExpression::copy() const
	{
		return new ArgumentExpression{ *this };
	}

	void ArgumentExpression::generate(Allocation*)
	{
#pragma message("TODO: compiler code generation for argument expression")
	}

	Expression* ArgumentExpression::optimize()
	{
		// No optimizations possible for an argument expression
		return nullptr;
	}

	Node* ArgumentExpression::evaluate(Context* context) const
	{
		return identifier->copy();
	}
}