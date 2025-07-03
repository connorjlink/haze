import std;

#include "MemberDeclarationExpression.h"

// Haze MemberDeclarationExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::uint16_t MemberDeclarationExpression::member_size() const
	{
		return type->size();
	}

	ExpressionType MemberDeclarationExpression::etype() const
	{
		return ExpressionType::STRUCT_MEMBER_DECLARATION;
	}

	MemberDeclarationExpression* MemberDeclarationExpression::copy() const
	{
		return new MemberDeclarationExpression{ *this };
	}

	void MemberDeclarationExpression::generate(Allocation*)
	{
		// No direct code generation for a struct member declaration
	}

	Expression* MemberDeclarationExpression::optimize()
	{
		// No optimizations possible for a struct member declaration
		return nullptr;
	}

	Node* MemberDeclarationExpression::evaluate(Context*) const
	{
		// No interpreter evaluation for a struct member declaration
		return nullptr;
	}
}
