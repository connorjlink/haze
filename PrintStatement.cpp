#include "PrintStatement.h"
#include "Expression.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"

// Haze PrintStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType PrintStatement::stype() const
	{
		return StatementType::PRINT;
	}

	PrintStatement* PrintStatement::copy() const
	{
		return new PrintStatement{ *this };
	}

	void PrintStatement::generate(Allocation*)
	{
		AutoStackAllocation temp{};
		message->generate(temp.source());
#pragma message("TODO: ensure that make_message() is called when generating string literals")
		_generator->print_message(temp.source()->read());
	}

	Statement* PrintStatement::optimize()
	{
		// No optimizations possible for a print statement
		return nullptr;
	}

	Node* PrintStatement::evaluate(Context* context) const
	{
		const auto value = harvest(message->evaluate(context));
		context->print(format(value));
		return nullptr;
	}
}