import std;

#include <ast/ArgumentExpression.h>
#include <ast/IdentifierExpression.h>
#include <type/Type.h>

// Haze ArgumentExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType ArgumentExpression::etype() const
	{
		return ExpressionType::ARGUMENT;
	}

	TypeType ArgumentExpression::ttype() const
	{
		return type->ttype();
	}

	ArgumentExpression* ArgumentExpression::copy() const
	{
		return new ArgumentExpression{ *this };
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