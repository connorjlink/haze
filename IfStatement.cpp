#include "IfStatement.h"

#include "Allocation.h"
#include "Allocator.h"

#include "Utility.h"

#include <format>

namespace hz
{
	Statement::Type IfStatement::stype() const
	{
		return Statement::Type::IF;
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
		// we need to force allocate this one :(
		auto condition_allocation = allocator->allocate_static(DC, true);

		// TODO: finish if statement generation here
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

		return new IfStatement{ condition_optimized, if_body_optimized, else_body_optimized };
	}
}