#ifndef HAZE_TYPEDEFNAMETYPE_H
#define HAZE_TYPEDEFNAMETYPE_H

#include <type/TypeKind.h>

// Haze TypedefName.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypedefNameType : public TypeBase
	{
	public:
		TypeHandle type;
		std::string name;

	public:
		TypeKind type_kind() const
		{
			return TypeKind::TYPEDEF_NAME;
		}

		Offset size() const
		{
			return type.size();
		}

		bool is_complete() const
		{
			return type.is_complete();
		}
	};
}

#endif
