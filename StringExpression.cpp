#include "StringExpression.h"
#include "ErrorReporter.h"
#include "Allocation.h"
#include "Generator.h"

// Haze StringExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType StringExpression::etype() const
	{
		return ExpressionType::STRING;
	}

	StringExpression* StringExpression::copy() const
	{
		return new StringExpression{ *this };
	}

	void StringExpression::generate(Allocation* allocation)
	{
		_error_reporter->post_error("unsupported compiler expression type `string`", _token);
		_generator->make_copy(allocation->read(), 0xEE);
	}

	Expression* StringExpression::optimize()
	{
		// No optimizations possible for a string expression
		return nullptr;
	}

	Node* StringExpression::evaluate(Context* context) const
	{
		// HACK: const_cast is pretty terrible to use 
		// but it's probably OK here since we know that the caller will not mutate this
		//return const_cast<StringExpression*>(this);

#pragma message("TODO: is this the same as just returning the current object???")
		return copy();
	}
}
