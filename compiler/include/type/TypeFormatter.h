#ifndef HAZE_TYPEFORMATTER_H
#define HAZE_TYPEFORMATTER_H

#include <type/defs/TypePrecedence.h>

// Haze TypeFormatter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypeBase;

	TypePrecedence precedence(const TypeBase& type);

#define DECLARE_TYPE_FORMATTER(x) std::string to_string(x type, const std::string& name = "<anonymous>", TypePrecedence parent_precedence = TypePrecedence::LOWEST)
	DECLARE_TYPE_FORMATTER(TypeHandle);

	// forward declaration required not to form circular dependency with the incomplete type facade
#define X(enumerator, precedence, type, name) class type;
	TYPE_KINDS(X)
#undef X

#define X(enumerator, precedence, type, name) DECLARE_TYPE_FORMATTER(const type&);
	TYPE_KINDS(X)
#undef X
}

#endif
