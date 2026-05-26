import std;

#include <allocator/Allocation.h>
#include <allocator/Allocator.h>
#include <ast/StringExpression.h>
#include <command/models/IntegerLiteral.h>
#include <toolchain/Generator.h>

// Haze StringExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType StringExpression::etype() const
	{
		return ExpressionType::STRING;
	}

	TypeKind StringExpression::tag_type() const
	{
		return TypeKind::STRING;
	}

	void StringExpression::generate(ValueHandle allocation)
	{
		// add an extra byte for the null terminator byte
		const auto bytes = message.length() + 1;

		const auto address = REQUIRE_SAFE(Allocator)->allocate(static_cast<Address>(bytes));
		const auto integer_literal = static_cast<BigInteger>(address);

		REQUIRE_SAFE(Generator)->make_message(address, message);
		REQUIRE_SAFE(Generator)->make_immediate(allocation->read(), integer_literal);
	}

	ExpressionHandle StringExpression::optimize()
	{
		// No optimizations possible for a string expression
		return nullptr;
	}

	Node* StringExpression::evaluate(Context* context) const
	{
		// the caller might mutate this, so the result must be a copy
		return copy();
	}
}
