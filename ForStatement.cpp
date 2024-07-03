#include "ForStatement.h"

#include "Allocation.h"
#include "Allocator.h"

#include <format>

namespace hz
{
	Statement::Type ForStatement::stype() const
	{
		return Statement::Type::FOR;
	}

	std::string ForStatement::string() const
	{
		return std::format("for statement ({}; {}; {}) \n[\n{}\n]\n", 
			initialization->string(), condition->string(), expression->string(), body->string());
	}

	ForStatement* ForStatement::copy() const
	{
		return new ForStatement{ *this };
	}

	void ForStatement::generate(Allocation*)
	{
		// we need to force allocate this one :(
		auto condition_allocation = allocator->allocate_static(DC, true);

		// TODO: finish for statement generation
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

		return new ForStatement{ initialization_optimized, condition_optimized, expression_optimized, body_optimized };
	}
}