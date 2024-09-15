#include "ExitStatement.h"
#include "Generator.h"
#include "Allocation.h"
#include "Expression.h"
#include "Context.h"
#include "Evaluator.h"
#include "RegisterAllocator.h"

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
		auto allocation = new AutoStackAllocation{};
		_code->generate(allocation);
		_generator->make_exit(allocation->allocation);
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
		context->exit(harvest(value));
	}

}