#ifndef HAZE_TYPEFORMATTER_H
#define HAZE_TYPEFORMATTER_H

#include <type/defs/TypePrecedence.h>

// Haze TypeFormatter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypeBase;

	TypePrecedence precedence(const TypeBase& type);

	std::string format_type(TypeHandle type, const std::string& name = "<anonymous>", TypePrecedence parent_predence = TypePrecedence::LOWEST);
}

#endif
