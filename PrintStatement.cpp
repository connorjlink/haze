import std;

#include "PrintStatement.h"
#include "Expression.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"
#include "ErrorReporter.h"

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

		using enum ExpressionType;
		switch (message->etype())
		{
			case STRING: _generator->print_message(temp.source()->read()); break;
			case INTEGER_LITERAL: _generator->print_number(temp.source()->read()); break;
			default: _error_reporter->post_error("unsupported compiler print expression type", message->_token); return;
		}
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