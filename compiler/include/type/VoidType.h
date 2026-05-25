#ifndef HAZE_VOIDTYPE_H
#define HAZE_VOIDTYPE_H

#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeKind.h>

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
		TypeKind type_kind() const
		{
			return TypeKind::VOID;
		}

		Offset size() const
		{
			return 0;
		}

		std::string string() const
		{
			return std::format("{} {} void", 
				storage,
				qualifier);
		}

		bool is_complete() const
		{
			return false;
		}
	};
}

#endif
