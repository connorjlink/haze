import std;

#include <ast/NullStatement.h>

// Haze NullStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementKind NullStatement::stype() const
	{
		return StatementKind::NUL;
	}

	void NullStatement::generate(ValueHandle)
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