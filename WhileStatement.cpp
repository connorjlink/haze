import std;

#include "WhileStatement.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Generator.h"
#include "RandomUtility.h"
#include "ErrorReporter.h"

// Haze WhileStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType WhileStatement::stype() const
	{
		return StatementType::WHILE;
	}

	WhileStatement* WhileStatement::copy() const
	{
		return new WhileStatement{ *this };
	}

	void WhileStatement::generate(Allocation*)
	{
		// 3 digits of randomness for now
		const auto uuid = hz::generate(3);

		const auto begin_while_label = std::format("begin_while_{:03d}", uuid);
		const auto end_while_label = std::format("end_while_{:03d}", uuid);

		_generator->branch_label(begin_while_label);

		// scoping so that the while body can re-use the condition register
		{
			AutoStackAllocation condition_allocation{};
			condition->generate(condition_allocation.source());

			_generator->check_ifz(end_while_label, condition_allocation.source()->read());
		}

		body->generate();

		_generator->goto_command(begin_while_label);

		_generator->branch_label(end_while_label);
	}

	Statement* WhileStatement::optimize()
	{
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto body_optimized = AS_STATEMENT(body->optimize());

		if (condition_optimized == nullptr &&
			body_optimized == nullptr)
		{
			return nullptr;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (body_optimized == nullptr)
		{
			body_optimized = body;
		}

		return new WhileStatement{ condition_optimized, body_optimized, _token };
	}

	Node* WhileStatement::evaluate(Context* context) const
	{
		auto condition_evaluated = condition->evaluate(context);
		
		if (AS_EXPRESSION(condition_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("`while` loop conditions must evaluate to an r-value", condition->_token);
			return nullptr;
		}

		while (std::get<std::uint32_t>(harvest(condition_evaluated)) != 0)
		{
			body->evaluate(context);
			condition_evaluated = condition->evaluate(context);
		}

		return nullptr;
	}
}