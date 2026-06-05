#ifndef HAZE_TYPEDEFNAMETYPE_H
#define HAZE_TYPEDEFNAMETYPE_H

#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze TypedefNameType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypedefNameType : public TypeBase
	{
	public:
		TypeHandle type;
		std::string name;

	public:
		Offset size() const;
		bool is_complete() const;
	};
}

#endif
