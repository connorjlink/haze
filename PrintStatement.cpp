#include "PrintStatement.h"

#include <format>

namespace hz
{
	StatementType PrintStatement::stype() const
	{
		return StatementType::NONE;
	}

	std::string PrintStatement::string() const
	{
		return std::format("print statement");
	}

	PrintStatement* PrintStatement::copy() const
	{
		return new PrintStatement{ *this };
	}

	void PrintStatement::generate(Allocation*)
	{
		Log::error("Print statements cannot be generated in a compiled context");
	}

	Statement* PrintStatement::optimize()
	{
		// No optimizations possible for a print statement
		return nullptr;
	}

	Node* PrintStatement::evaluate(Context* context) const
	{
		context->print(message);
		return nullptr;
	}
}