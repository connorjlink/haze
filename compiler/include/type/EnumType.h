#ifndef HAZE_ENUMTYPE_H
#define HAZE_ENUMTYPE_H

#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze EnumType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct EnumType : public TypeBase
	{
	public:
		struct EnumMember
		{
			std::string name;
			BigInteger value;
		};

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		std::optional<std::vector<EnumMember>> members;
		std::string tag;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;
	};
}

#endif
