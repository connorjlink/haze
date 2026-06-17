#ifndef HAZE_TYPEDEFNAMETYPE_H
#define HAZE_TYPEDEFNAMETYPE_H

#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze TypedefNameType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct TypedefNameType : public TypeBase
	{
	public:
		TypeHandle type;
		std::string_view name;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;

	public:
		TypedefNameType(TypeHandle type, std::string_view name)
			: type{ type }, name{ name }
		{
		}
	};
}

#endif
