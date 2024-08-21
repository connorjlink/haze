#include "AdjustExpression.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "Context.h"
#include "Evaluator.h"

#include <format>

namespace hz
{
	ExpressionType AdjustExpression::etype() const
	{
		return ExpressionType::ADJUST;
	}

	std::string AdjustExpression::string() const
	{
		const auto operation = increment ? "increment" : "decrement";
		return std::format("{} expression", operation);
	}

	AdjustExpression* AdjustExpression::copy() const
	{
		return new AdjustExpression{ *this };
	}

	void AdjustExpression::generate(Allocation*)
	{
#pragma message("TODO: implement increment/decrement for compiling")
		Log::error("");
	}

	Expression* AdjustExpression::optimize()
	{
		// No optimizations possible for an adjustment expression
		return nullptr;
	}

	Node* AdjustExpression::evaluate(Context* context) const
	{
		if (target->etype() == ExpressionType::INTEGER_LITERAL)
		{
			auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(target);
			
			if (increment) 
			{
				integer_literal_expression->value++;
			}

			else
			{
				integer_literal_expression->value--;
			}

			return integer_literal_expression;
		}

		else if (target->etype() == ExpressionType::IDENTIFIER)
		{
			auto identifier_expression = AS_IDENTIFIER_EXPRESSION(target);
			auto identifier_expression_evaluated = identifier_expression->evaluate(context);

			auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(identifier_expression_evaluated);

			if (increment)
			{
				integer_literal_expression->value++;
			}

			else
			{
				integer_literal_expression->value--;
			}

			context->define_variable(identifier_expression->name, harvest(integer_literal_expression));
			return integer_literal_expression;
		}

		Log::error("Adjustment expresion target must be an integer literal or identifier");
	}
}