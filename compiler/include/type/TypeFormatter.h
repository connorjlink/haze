#ifndef HAZE_TYPEFORMATTER_H
#define HAZE_TYPEFORMATTER_H

#include <type/defs/TypePrecedence.h>

// Haze TypeFormatter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypeBase;

	TypePrecedence precedence(const TypeBase& type);

#define METHOD_SIGNATURE(x) std::string to_string(x type, const std::string& name = "<anonymous>", TypePrecedence parent_predence = TypePrecedence::LOWEST);
	METHOD_SIGNATURE(TypeHandle)

	// forward declaration required not to form circular dependency with the incomplete type facade
#define X(enumerator, precedence, type, name) class type;
	TYPE_KINDS(X)
#undef X

#define X(enumerator, precedence, type, name) METHOD_SIGNATURE(const type&)
	TYPE_KINDS(X)
#undef X

#undef METHOD_SIGNATURE
}

#endif
