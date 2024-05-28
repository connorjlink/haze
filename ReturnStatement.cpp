#include "ReturnStatement.h"
#include "Function.h"
#include "IntegerLiteralExpression.h"
#include "Parser.h"
#include "Log.h"

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
		if (value == nullptr)
		{
			if (AS_FUNCTION_SYMBOL(parser->reference_symbol(Symbol::Type::FUNCTION, enclosing_function))->return_type
				
				parent->return_type != ReturnType::NVR)
			{
				Log::error("no return value was specified for a non-void-returning function");
			}

			value = new IntegerLiteralExpression{ 0 };
		}

		value->generate(received_allocation);
	}

	Statement* ReturnStatement::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new ReturnStatement{ enclosing_function, AS_EXPRESSION(value_optimized), allocation };
		}

		return nullptr;
	}
}
