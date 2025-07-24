import std;

#include <ast/Statement.h>

// Haze Statement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	NodeType Statement::ntype() const
	{
		return NodeType::STATEMENT;
	}
}
