#ifndef HAZE_TYPEDEFNAMETYPE_H
#define HAZE_TYPEDEFNAMETYPE_H

#include <type/Type.h>

// Haze TypedefName.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypedefName : public TypeBase
	{
	public:
		Handle type;
		std::string name;

	public:
		TypeKind ttype() const
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
