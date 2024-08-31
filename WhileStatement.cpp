#include "WhileStatement.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Generator.h"
#include "RandomUtility.h"
#include "ErrorReporter.h"

#include <format>

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
		// we need to force allocate this one :(
		ManagedStaticAllocation condition_allocation{ DC, true };

		// 3 digits of randomness for now
		const auto loop_uuid = hz::generate(3);

		const auto start_label = std::format("start_while_{:02d}", loop_uuid);
		const auto end_label = std::format("end_while_{:02d}", loop_uuid);

		_generator->label(start_label);

		body->generate();

		condition->generate(condition_allocation.allocation);
		
		// unfortunately our processor's limitation also mean we need to force this mess, too :(
		ManagedStaticAllocation temp{ condition_allocation.allocation->read(), true };
#pragma message("TODO: code generation for while")

		// TODO: finish while statement codegen here!
		_generator->make_brnz(start_label, condition_allocation.allocation->read());
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