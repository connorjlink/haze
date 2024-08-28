#include "IfStatement.h"
#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Allocator.h"
#include "Utility.h"
#include "Evaluator.h"
#include "Log.h"

#include <format>

namespace hz
{
	StatementType IfStatement::stype() const
	{
		return StatementType::IF;
	}

	std::string IfStatement::string() const
	{
		return std::format("if statement ({}) \n[\n{}\n]\n else \n[\n{}\n]\n",
			condition->string(), if_body->string(), else_body ? else_body->string() : "");
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

		// scoping so that the if body can use our condition register
		{
			// we need to force allocate this one :(
			ManagedStaticAllocation condition_allocation{ DC, true };

			condition->generate(condition_allocation.allocation);

			_generator->make_bool(condition_allocation.allocation->read());
			_generator->make_brnz(begin_else_label, condition_allocation.allocation->read());
		}

		_generator->label(begin_if_label);

		if_body->generate();

		if (else_body != nullptr)
		{
			ManagedStaticAllocation temp{};
			_generator->make_copy(temp.allocation->read(), 1);
			_generator->make_brnz(end_else_label, temp.allocation->read());
		}
		
		_generator->label(begin_else_label);

		if (else_body != nullptr)
		{
			else_body->generate();
		}

		_generator->label(end_else_label);
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

		auto condition_optimized_evaluated = AS_EXPRESSION(condition_optimized->evaluate(_context));

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

		return new IfStatement{ condition_optimized, if_body_optimized, else_body_optimized };
	}

	Node* IfStatement::evaluate(Context* context) const
	{
		auto condition_evaluated = condition->evaluate(context);

		if (AS_EXPRESSION(condition_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			Log::error("'if' statement conditions must evaluate to an integer");
		}

		if (std::get<std::uint32_t>(harvest(condition_evaluated)) != 0)
		{
			DISCARD if_body->evaluate(context);
		}

		else
		{
			if (else_body != nullptr)
			{
				DISCARD else_body->evaluate(context);
			}
		}

		return nullptr;
	}
}