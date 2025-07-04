import std;

#include <ast/AdjustExpression.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/IdentifierExpression.h>
#include <runtime/Context.h>
#include <runtime/Evaluator.h>
#include <toolchain/Generator.h>
#include <allocator/Allocation.h>
#include <error/ErrorReporter.h>

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

		if (increment)
		{
			REQUIRE_SAFE(Generator)->compute_increment(allocation->read(), allocation->read());
		}

		else
		{
			REQUIRE_SAFE(Generator)->compute_decrement(allocation->read(), allocation->read());
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

			context->define_variable(identifier_expression->name, node_to_variable(integer_literal_expression));
			return integer_literal_expression;
		}

		USE_SAFE(ErrorReporter)->post_error("adjustment expression target must result in a modifiable l-value", target->_token);
		return nullptr;
	}
}