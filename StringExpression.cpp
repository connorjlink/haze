#include "StringExpression.h"
#include "ErrorReporter.h"
#include "Allocation.h"
#include "Generator.h"
#include "RuntimeAllocator.h"

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
		// add an extra byte for the null terminator byte
		const auto bytes = message.length() + 1;

		const auto address = _runtime_allocator->allocate(static_cast<std::uint32_t>(bytes));
		_generator->make_message(address, message);
		_generator->make_immediate(allocation->read(), address);
	}

	Expression* StringExpression::optimize()
	{
		// No optimizations possible for a string expression
		return nullptr;
	}

	Node* StringExpression::evaluate(Context* context) const
	{
		// NOTE: old method--could be more efficient since we technically a copy of `this` is not actually needed
		// HACK: const_cast is pretty terrible to use 
		// but it's probably OK here since we know that the caller will not mutate this
		//return const_cast<StringExpression*>(this);

		return copy();
	}
}
