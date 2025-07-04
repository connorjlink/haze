import std;

#include <ast/MemberDeclarationExpression.h>
#include <ast/StructDeclarationStatement.h>

// Haze StructDeclarationStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::uint16_t StructDeclarationStatement::members_size() const
	{
		auto sum = std::uint16_t{ 0 };

		for (auto member : members)
		{
			sum += member->member_size();
		}

		return sum;
	}

	std::uint16_t StructDeclarationStatement::struct_size() const
	{
#pragma message("TODO: compute actual struct bytes size. Alignment?")
		return -1;
	}

	StatementType StructDeclarationStatement::stype() const
	{
		return StatementType::STRUCT;
	}

	StructDeclarationStatement* StructDeclarationStatement::copy() const
	{
		return new StructDeclarationStatement{ *this };
	}

	void StructDeclarationStatement::generate(Allocation*)
	{
		// No direct code generation for a struct declaration
	}

	Statement* StructDeclarationStatement::optimize()
	{
		// No optimizations possible for a struct declaration
		return nullptr;
	}

	Node* StructDeclarationStatement::evaluate(Context*) const
	{
		// No interpreter evaluation for a struct declaration
		return nullptr;
	}
}
