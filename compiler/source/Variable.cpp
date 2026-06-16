import std;

#include <ast/expression/Expression.h>
#include <runtime/Variable.h>
#include <type/Type.h>

// Haze Variable.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StructOrUnionVariable::StructOrUnionVariable(TypeHandle type, std::unordered_map<std::string, ExpressionHandle> members)
		: VariableBase{ type }, members{ std::move(members) }
	{
	}
}
