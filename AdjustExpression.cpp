#include "AdjustExpression.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "Context.h"
#include "Evaluator.h"
#include "Generator.h"
#include "Allocation.h"
#include "ErrorReporter.h"

#include <format>

// Haze AdjustExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType AdjustExpression::etype() const
	{
		return ExpressionType::ADJUST;
	}

	AdjustExpression* AdjustExpression::copy() const
	{
		return new AdjustExpression{ *this };
	}

	void AdjustExpression::generate(Allocation* allocation)
	{
		target->generate(allocation);
		ManagedStaticAllocation temp{ allocation->read(), true };
		_generator->make_copy(temp.allocation->read(), 1);

		if (increment)
		{
			_generator->make_iadd(allocation->read(), temp.allocation->read());
		}

		else
		{
			_generator->make_isub(allocation->read(), temp.allocation->read());
		}
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

		_error_reporter->post_error("adjustment expression target must result in a modifiable l-value", target->_token);
		return nullptr;
	}
}