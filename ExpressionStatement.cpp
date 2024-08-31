#include "ExpressionStatement.h"
#include "Allocation.h"

#include <format>

// Haze ExpressionStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType ExpressionStatement::stype() const
	{
		return StatementType::EXPRESSION;
	}

	ExpressionStatement* ExpressionStatement::copy() const
	{
		return new ExpressionStatement{ *this };
	}

	void ExpressionStatement::generate(Allocation*)
	{
		ManagedStaticAllocation temp{};
		expression->generate(temp.allocation);
	}

	Statement* ExpressionStatement::optimize()
	{
		auto expression_optimized = AS_EXPRESSION(expression->optimize());

		if (expression_optimized == nullptr)
		{
			return nullptr;
		}

		return new ExpressionStatement{ expression_optimized, _token };
	}

	Node* ExpressionStatement::evaluate(Context* context) const
	{
#pragma message("TODO: is it alright to discard here???")
		expression->evaluate(context);
		return nullptr;
	}
}