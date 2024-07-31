#include "ExpressionStatement.h"

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
		// TODO: expression statement codegen here!
	}

	Statement* ExpressionStatement::optimize()
	{
		auto expression_optimized = AS_EXPRESSION(expression->optimize());

		if (expression_optimized == nullptr)
		{
			return nullptr;
		}

		return new ExpressionStatement{ expression_optimized };
	}
}