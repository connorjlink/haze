
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
	X(CHAR, char) \
	X(SHORT, short) \
	X(INT, int) \
	X(LONG, long) \
	X(LONG_LONG, long long)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case IntKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, INT_KINDS, IntKind, int kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER


	class IntType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		// C defaults to signed
		TypeSignedness signedness = TypeSignedness::SIGNED;
		IntKind int_kind;

	public:
		TypeKind type_knid() const
		{
			return TypeKind::INT;
		}

		Offset size() const
		{
			switch (int_kind)
			{
#define SWITCH_CASE(enumerator, name) case IntKind::enumerator: return sizeof(name);
				INT_KINDS(SWITCH_CASE)
#undef SWITCH_CASE
			}

			CommonErrors::invalid_int_type(int_kind, NULL_TOKEN);
			return -1;
		}

		bool is_complete() const
		{
			return true;
		}
	};
}

#endif
