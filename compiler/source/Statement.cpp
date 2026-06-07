import std;

#include <ast/statement/Statement.h>
#include <type/Type.h>

// Haze Statement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	//////////////////////////////////////////////////////
	// Null Statement
	//////////////////////////////////////////////////////

	std::string NullStatement::format(void) const
	{
		return std::format(";");
	}

	void NullStatement::generate(const Storage&) const
	{
		// no code generation necessary for null statement
	}

	StatementHandle NullStatement::evaluate(const Storage& storage, Context&) const
	{
		// no evaluation necessary for null statement
		return make_invalid_handle<StatementSumDispatcher, StatementSumStorage>(storage);
	}

	StatementHandle NullStatement::optimize(const Storage& storage) const
	{
		// no optimizations possible for null statement
		return make_invalid_handle<StatementSumDispatcher, StatementSumStorage>(storage);
	}

	TypeHandle NullStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for null statement
		return make_invalid_handle<TypeSumDispatcher, TypeSumStorage>(storage);
	}
}
