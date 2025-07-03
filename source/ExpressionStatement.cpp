import std;

#include "ExpressionStatement.h"
#include "Allocation.h"

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
		AutoStackAllocation temp{};
		expression->generate(temp.source());
	}

	Statement* ExpressionStatement::optimize()
	{
		if (auto expression_optimized = AS_EXPRESSION(expression->optimize()))
		{
			return new ExpressionStatement{ expression_optimized, _token };
		}

		return nullptr;
	}

	Node* ExpressionStatement::evaluate(Context* context) const
	{
		expression->evaluate(context);
		return nullptr;
	}
}