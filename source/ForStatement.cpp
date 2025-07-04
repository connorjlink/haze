import std;

#include <allocator/Allocation.h>
#include <ast/ForStatement.h>
#include <ast/NullStatement.h>
#include <ast/IntegerLiteralExpression.h>
#include <runtime/Context.h>
#include <utility/Random.h>

// Haze ForStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType ForStatement::stype() const
	{
		return StatementType::FOR;
	}

	ForStatement* ForStatement::copy() const
	{
		return new ForStatement{ *this };
	}

	void ForStatement::generate(Allocation*)
	{
		// 3 digits of randomness for now
		const auto uuid = hz::generate(3);

		const auto begin_for_label = std::format("begin_for_{:03d}", uuid);
		const auto end_for_label = std::format("end_for_{:03d}", uuid);

		initialization->generate();

		REQUIRE_SAFE(Generator)->branch_label(begin_for_label);

		// scoping so that the for body can re-use the condition register
		{
			AutoStackAllocation condition_allocation{};
			condition->generate(condition_allocation.source());

			REQUIRE_SAFE(Generator)->check_ifnz(end_for_label, condition_allocation.source()->read());
		}

		body->generate();
		expression->generate();

		REQUIRE_SAFE(Generator)->goto_command(begin_for_label);

		REQUIRE_SAFE(Generator)->branch_label(end_for_label);
	}

	Statement* ForStatement::optimize()
	{
		auto initialization_optimized = AS_STATEMENT(initialization->optimize());
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto expression_optimized = AS_EXPRESSION(expression->optimize());
		auto body_optimized = AS_STATEMENT(body->optimize());

		if (initialization_optimized == nullptr &&
			condition_optimized == nullptr &&
			expression_optimized == nullptr &&
			body_optimized == nullptr)
		{
			return nullptr;
		}

		if (initialization_optimized == nullptr)
		{
			initialization_optimized = initialization;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (expression_optimized == nullptr)
		{
			expression_optimized = expression;
		}

		if (body_optimized == nullptr)
		{
			body_optimized = body;
		}

		auto condition_optimized_evaluated = AS_EXPRESSION(condition_optimized->evaluate(USE_SAFE(Context).get()));

		if (condition_optimized_evaluated != nullptr)
		{
			auto value = AS_INTEGER_LITERAL_EXPRESSION(condition_optimized_evaluated)->value;

			// The value is constexpr false so we don't even need to run the loop
			if (value == 0)
			{
				return new NullStatement{ _token };
			}
		}

		return new ForStatement{ initialization_optimized, condition_optimized, expression_optimized, body_optimized, _token };
	}

	Node* ForStatement::evaluate(Context* context) const
	{
		initialization->evaluate(context);

		auto condition_evaluated = AS_EXPRESSION(condition->evaluate(context));

		if (condition->etype() != ExpressionType::INTEGER_LITERAL)
		{
			USE_SAFE(ErrorReporter)->post_error("`for` loop conditions must result in an r-value", condition_evaluated->_token);
			return nullptr;
		}

		auto integer_literal = AS_INTEGER_LITERAL_EXPRESSION(condition_evaluated)->value;

		while (integer_literal_raw(integer_literal) != EI(0))
		{
			body->evaluate(context);
			expression->evaluate(context);

			condition_evaluated = AS_EXPRESSION(condition->evaluate(context));
			integer_literal = AS_INTEGER_LITERAL_EXPRESSION(condition_evaluated)->value;
		}

		return nullptr;
	}
}