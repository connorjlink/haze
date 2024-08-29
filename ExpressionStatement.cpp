#include "ExpressionStatement.h"
#include "Utility.h"
#include "Allocation.h"

#include <format>

namespace hz
{
	StatementType ExpressionStatement::stype() const
	{
		return StatementType::EXPRESSION;
	}

	std::string ExpressionStatement::string() const
	{
		return std::format("expression statement ({})", 
			expression->string());
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
		DISCARD expression->evaluate(context);
		return nullptr;
	}
}