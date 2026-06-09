
#ifndef HAZE_INTTYPE_H
#define HAZE_INTTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeSignedness.h>
#include <type/defs/TypeKind.h>
#include <utility/AutoEnum.h>

// Haze IntType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define INT_KINDS(X) \
	X(CHAR,      char) \
	X(SHORT,     short) \
	X(INT,       int) \
	X(LONG,      long) \
	X(LONG_LONG, long long)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case IntKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, IntKind::enumerator },
#define FORWARD_DECLARATION(enumerator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, INT_KINDS, IntKind, int kind)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER


	struct IntType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		// C defaults to signed
		TypeSignedness signedness = TypeSignedness::SIGNED;
		IntKind int_kind;

	public:
		Offset size() const;
		bool is_complete() const;
	};
}

#endif
