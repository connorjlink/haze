
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


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, IntKind)

	DEFINE_ENUM(INT_KINDS, IntKind, int kind)

#undef AUTOENUM_ROUTER


	struct IntType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		// C defaults to signed
		TypeSignedness signedness = TypeSignedness::SIGNED;
		IntKind int_kind;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;

	public:
		IntType(StorageClass storage, TypeQualifier qualifier, TypeSignedness signedness, IntKind int_kind)
			: storage{ storage }, qualifier{ qualifier }, signedness{ signedness }, int_kind{ int_kind }
		{
		}
	};
}

#endif
