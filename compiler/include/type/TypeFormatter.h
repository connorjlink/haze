#ifndef HAZE_TYPEFORMATTER_H
#define HAZE_TYPEFORMATTER_H

#include <type/defs/TypePrecedence.h>

// Haze TypeFormatter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct TypeBase;

#define _PRIVATE_DECLARE_TYPE_FORMATTER(x, defaultname, defaultprecedence) std::string to_string(x type, const std::string& name defaultname, TypePrecedence parent_precedence defaultprecedence)
#define DECLARE_TYPE_FORMATTER(x) \
	_PRIVATE_DECLARE_TYPE_FORMATTER(x, = "<anonymous>", = TypePrecedence::LOWEST)
#define DEFINE_TYPE_FORMATTER(x) \
	_PRIVATE_DECLARE_TYPE_FORMATTER(x, , )

	DECLARE_TYPE_FORMATTER(TypeHandle);

	// forward declaration required not to form circular dependency with the incomplete type facade
#define X(enumerator, precedence, type, name) struct type;
	TYPE_KINDS(X)
#undef X

#define X(enumerator, precedence, type, name) DECLARE_TYPE_FORMATTER(const type&);
	TYPE_KINDS(X)
#undef X
}

#endif
