#include "NullStatement.h"
#include "Generator.h"

#include <format>

namespace hz
{
	StatementType NullStatement::stype() const
	{
		return StatementType::NONE;
	}

	std::string NullStatement::string() const
	{
		return std::format("null statement");
	}

	NullStatement* NullStatement::copy() const
	{
		return new NullStatement{ *this };
	}

	void NullStatement::generate(Allocation*)
	{
		_generator->make_move(R0, R0);
	}

	Statement* NullStatement::optimize()
	{
		// No optimizations possible for a null
		return nullptr;
	}

	Node* NullStatement::evaluate(Context* context) const
	{
		return nullptr;
	}
}