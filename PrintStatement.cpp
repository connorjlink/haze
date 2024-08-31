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
		ManagedStaticAllocation temp{};
		message->generate(temp.allocation);
		// NOTE: $1111 is the screen buffer address
		// it is flushed once per clock cycle
		_generator->make_save(0x1111, temp.allocation->read());
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