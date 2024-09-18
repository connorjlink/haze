#include "ArgumentExpression.h"
#include "ErrorReporter.h"

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
		_error_reporter->post_error("unsupported compiler expression type `argument`", _token);
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