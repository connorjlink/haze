import std;

#include "ExitStatement.h"
#include "Generator.h"
#include "Allocation.h"
#include "Expression.h"
#include "Context.h"
#include "Evaluator.h"
#include "StackAllocator.h"

// Haze ExitStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType ExitStatement::stype() const
	{
		return StatementType::EXIT;
	}
	
	ExitStatement* ExitStatement::copy() const
	{
		return new ExitStatement{ *this };
	}

	void ExitStatement::generate(Allocation*)
	{
		AutoStackAllocation temp{};
		_code->generate(temp.source());
		_generator->exit_program(temp.source()->read());
	}

	Statement* ExitStatement::optimize()
	{
		if (auto code_optimized = _code->optimize();
			code_optimized != nullptr)
		{
			return new ExitStatement{ AS_EXPRESSION(code_optimized), _token };
		}

		return nullptr;
	}

	Node* ExitStatement::evaluate(Context* context) const
	{
		auto value = _code->evaluate(context);
		context->exit_program(node_to_variable(value));
		return nullptr;
	}

}