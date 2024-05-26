#include "ReturnStatement.h"

#include <format>

namespace hz
{
	Statement::Type ReturnStatement::stype() const
	{
		return Statement::Type::RETURN;
	}

	std::string ReturnStatement::string() const
	{
		return std::format("return ({})", value->string());
	}

	ReturnStatement* ReturnStatement::copy() const
	{
		return new ReturnStatement{ *this };
	}

	void ReturnStatement::generate(Allocation* received_allocation)
	{
		//TODO: if value == nullptr, then the function MUST have `void` return type
		if (value)
		{
			value->generate(received_allocation);
		}
	}

	Statement* ReturnStatement::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new ReturnStatement{ AS_EXPRESSION(value_optimized), allocation };
		}

		return nullptr;
	}
}