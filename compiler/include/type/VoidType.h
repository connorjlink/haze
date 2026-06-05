#ifndef HAZE_VOIDTYPE_H
#define HAZE_VOIDTYPE_H

#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze VoidType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class VoidType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;

	public:
		Offset size() const;
		bool is_complete() const;
	};
}

#endif
