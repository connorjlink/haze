#ifndef HAZE_FLOATTYPE_H
#define HAZE_FLOATTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeKind.h>
#include <utility/AutoEnum.h>
#include <utility/Constants.h>

// Haze FloatType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define FLOAT_KINDS(X) \
	X(FLOAT, float) \
	X(DOUBLE, double) \
	X(LONG_DOUBLE, long double)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case FloatKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, FloatKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FLOAT_KINDS, FloatKind, float kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER


	class FloatType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		FloatKind float_kind;

	public:
		Offset size() const;
		bool is_complete() const;
	};
}

#endif
