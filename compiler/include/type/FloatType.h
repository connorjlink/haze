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
	X(FLOAT,       float) \
	X(DOUBLE,      double) \
	X(LONG_DOUBLE, long double)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, FloatKind)

	DEFINE_ENUM(FLOAT_KINDS, FloatKind, float kind)

#undef AUTOENUM_ROUTER


	struct FloatType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		FloatKind float_kind;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;

	public:
		FloatType(StorageClass storage, TypeQualifier qualifier, FloatKind float_kind)
			: storage{ storage }, qualifier{ qualifier }, float_kind{ float_kind }
		{
		}
	};
}

#endif
