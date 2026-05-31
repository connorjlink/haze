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

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, FLOAT_KINDS, FloatKind, float kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER


	class FloatType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		FloatKind float_kind;

	public:
		TypeKind type_kind() const
		{
			return TypeKind::FLOAT;
		}

		Offset size() const
		{
			switch (float_kind)
			{
#define X(enumerator, name) case FloatKind::enumerator: return sizeof(name);
				FLOAT_KINDS(X)
#undef X
			}

			CommonErrors::invalid_float_type(float_kind, NULL_TOKEN);
			return -1;
		}

		bool is_complete() const
		{
			return true;
		}
	};
}

#endif
