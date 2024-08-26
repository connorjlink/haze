#include "PrintStatement.h"
#include "Expression.h"
#include "Evaluator.h"
#include "Allocation.h"
#include "Generator.h"

#include <format>

namespace hz
{
	StatementType PrintStatement::stype() const
	{
		return StatementType::PRINT;
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
		ManagedStaticAllocation temp{};
		message->generate(temp.allocation);
		// NOTE: $FFFF is the screen buffer address
		// it is flushed once per clock cycle
		_generator->make_save(0xFFFF, temp.allocation->read());
	}

	Statement* PrintStatement::optimize()
	{
		// No optimizations possible for a print statement
		return nullptr;
	}

	Node* PrintStatement::evaluate(Context* context) const
	{
		const auto value = harvest(message->evaluate(context));
		context->print(std::format("{}", format(value)));
		return nullptr;
	}
}