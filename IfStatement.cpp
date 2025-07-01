import std;

#include "IfStatement.h"
#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Generator.h"
#include "Random.h"
#include "ErrorReporter.h"

// Haze IfStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType IfStatement::stype() const
	{
		return StatementType::IF;
	}

	IfStatement* IfStatement::copy() const
	{
		return new IfStatement{ *this };
	}

	void IfStatement::generate(Allocation*)
	{
		// 3 digits of randomness for now
		const auto uuid = hz::generate(3);

		const auto begin_if_label = std::format("begin_if_{:03d}", uuid);
		const auto begin_else_label = std::format("begin_else_{:03d}", uuid);
		const auto end_else_label = std::format("end_else_{:03d}", uuid);

		REQUIRE_SAFE(Generator)->branch_label(begin_if_label);

		// scoping so that the if body can re-use the condition register
		{
			AutoStackAllocation condition_allocation{};
			condition->generate(condition_allocation.source());

			REQUIRE_SAFE(Generator)->check_ifz(begin_else_label, condition_allocation.source()->read());
		}

		if_body->generate();

		if (else_body != nullptr)
		{
			REQUIRE_SAFE(Generator)->goto_command(end_else_label);
		}

		REQUIRE_SAFE(Generator)->branch_label(begin_else_label);

		if (else_body != nullptr)
		{
			else_body->generate();
			REQUIRE_SAFE(Generator)->branch_label(end_else_label);
		}
	}

	Statement* IfStatement::optimize()
	{
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto if_body_optimized = AS_STATEMENT(if_body->optimize());
		auto else_body_optimized = AS_STATEMENT(else_body ? else_body->optimize() : nullptr);

		if (condition_optimized == nullptr &&
			if_body_optimized == nullptr &&
			else_body_optimized == nullptr)
		{
			return nullptr;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (if_body_optimized == nullptr)
		{
			if_body_optimized = if_body;
		}

		if (else_body_optimized == nullptr)
		{
			else_body_optimized = else_body;
		}

		auto condition_optimized_evaluated = AS_EXPRESSION(condition_optimized->evaluate(REQUIRE_SAFE(Context).get()));

		if (condition_optimized_evaluated != nullptr)
		{
			auto value = AS_INTEGER_LITERAL_EXPRESSION(condition_optimized_evaluated)->value;

			// The condition is constexpr true, so run if branch only
			if (value != 0)
			{
				return if_body_optimized;
			}

			// otherwise run the else branch
			else
			{
				return else_body_optimized;
			}
		}

		return new IfStatement{ condition_optimized, if_body_optimized, else_body_optimized, _token };
	}

	Node* IfStatement::evaluate(Context* context) const
	{
		auto condition_evaluated = AS_EXPRESSION(condition->evaluate(context));

		if (condition_evaluated->etype() != ExpressionType::INTEGER_LITERAL)
		{
			USE_SAFE(ErrorReporter)->post_error("`if` statement conditions must result in an r-value", condition_evaluated->_token);
			return nullptr;
		}

		auto integer_literal = AS_INTEGER_LITERAL_EXPRESSION(condition_evaluated)->value;

		if (integer_literal_raw(integer_literal))
		{
			if_body->evaluate(context);
		}

		else
		{
			if (else_body != nullptr)
			{
				else_body->evaluate(context);
			}
		}

		return nullptr;
	}
}