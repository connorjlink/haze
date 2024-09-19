import std;

#include "NullStatement.h"
#include "InstructionEncoding.h"
#include "Generator.h"

// Haze NullStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType NullStatement::stype() const
	{
		return StatementType::NONE;
	}

	NullStatement* NullStatement::copy() const
	{
		return new NullStatement{ *this };
	}

	void NullStatement::generate(Allocation*)
	{
		// No code generated for a no-op
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