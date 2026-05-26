import std;

#include <ast/MemberDeclarationExpression.h>
#include <type/Type.h>

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

	TypeKind MemberDeclarationExpression::tag_type() const
	{
		return type->tag_type();
	}

	void MemberDeclarationExpression::generate(ValueHandle)
	{
		// No direct code generation for a struct member declaration
	}

	ExpressionHandle MemberDeclarationExpression::optimize()
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
