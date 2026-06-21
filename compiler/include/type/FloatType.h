#ifndef HAZE_FLOATTYPE_H
#define HAZE_FLOATTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/FloatKind.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeKind.h>
#include <type/defs/TypeQualifier.h>
#include <utility/AutoEnum.h>
#include <utility/Constants.h>

// Haze FloatType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
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
