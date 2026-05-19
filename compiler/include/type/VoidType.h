#ifndef HAZE_VOIDTYPE_H
#define HAZE_VOIDTYPE_H

#include <type/StorageClass.h>
#include <type/TypeQualifier.h>
#include <type/TypeKind.h>

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
		TypeKind ttype() const
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
				_storage_class_map.at(storage),
				_type_qualifier_map.at(qualifier));
		}

		bool is_complete() const
		{
			return false;
		}
	};
}

#endif
