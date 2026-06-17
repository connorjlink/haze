#ifndef HAZE_ARRAYTYPE_H
#define HAZE_ARRAYTYPE_H

#include <type/TypeFormatter.h>
#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze ArrayType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ArrayType : public TypeBase
	{
	public:
		TypeHandle element_type;
		std::optional<Address> length;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;

	public:
		ArrayType(TypeHandle element_type, std::optional<Address> length)
			: element_type{ element_type }, length{ length }
		{
		}
	};
}

#endif
