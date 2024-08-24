#include "ExpressionStatement.h"
#include "Utility.h"

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
#pragma message("TODO: expression statement codegen here!")
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

	Node* ExpressionStatement::evaluate(Context* context) const
	{
#pragma message("TODO: is it alright to discard here???")
		DISCARD expression->evaluate(context);
		return nullptr;
	}
}