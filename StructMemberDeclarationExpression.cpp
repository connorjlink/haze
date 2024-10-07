import std;

#include "StructMemberDeclarationExpression.h"

// Haze StructMemberDeclarationExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType StructMemberDeclarationExpression::etype() const
	{
		return ExpressionType::STRUCT_MEMBER_DECLARATION;
	}
}
