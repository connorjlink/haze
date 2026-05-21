import std;

#include <allocator/Allocation.h>
#include <ast/ExpressionStatement.h>

// Haze ExpressionStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementKind ExpressionStatement::stype() const
	{
		return StatementKind::EXPRESSION;
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