﻿#include "ReturnStatement.h"
#include "Function.h"
#include "IntegerLiteralExpression.h"
#include "Parser.h"
#include "Allocation.h"
#include "Allocator.h"
#include "Generator.h"
#include "Evaluator.h"
#include "ErrorReporter.h"
#include "Log.h"

#include <format>


namespace hz
{
	StatementType ReturnStatement::stype() const
	{
		return StatementType::RETURN;
	}

	std::string ReturnStatement::string() const
	{
		return std::format("return ({})", value->string());
	}

	ReturnStatement* ReturnStatement::copy() const
	{
		return new ReturnStatement{ *this };
	}

	void ReturnStatement::generate(Allocation*)
	{
		if (value == nullptr)
		{
			if (AS_FUNCTION_SYMBOL(_parser->reference_symbol(SymbolType::FUNCTION, enclosing_function, NULL_TOKEN))->return_type != ReturnType::NVR)
			{
				_error_reporter->post_error("no return value was specified for a non-`nvr` function", NULL_TOKEN);
				return;
			}

			value = new IntegerLiteralExpression{ 0 };
		}

		ManagedStaticAllocation temp{};
		value->generate(temp.allocation);
		_generator->fake_push(RETURNS, temp.allocation->read());
	}

	Statement* ReturnStatement::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new ReturnStatement{ enclosing_function, AS_EXPRESSION(value_optimized), allocation };
		}

		return nullptr;
	}

	Node* ReturnStatement::evaluate(Context* context) const
	{
		context->push_return(harvest(value->evaluate(context)));
		return nullptr;
	}
}
