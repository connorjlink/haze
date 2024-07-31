#include "NullStatement.h"

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
		// No code generated for a null
	}

	Statement* NullStatement::optimize()
	{
		// No optimizations possible for a null
		return nullptr;
	}
}