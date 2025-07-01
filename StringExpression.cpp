import std;

#include "StringExpression.h"
#include "Allocation.h"
#include "Generator.h"
#include "RuntimeAllocator.h"
#include "ErrorReporter.h"

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

		const auto address = REQUIRE_SAFE(RuntimeAllocator)->allocate(static_cast<std::uint32_t>(bytes));
		const auto integer_literal = make_integer_literal<platform_address_size>(address);

		REQUIRE_SAFE(Generator)->make_message(address, message);
		REQUIRE_SAFE(Generator)->make_immediate(allocation->read(), integer_literal);
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
