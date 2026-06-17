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
			std::string_view name;
			BigInteger value;
		};

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		std::optional<std::vector<EnumMember>> members;
		std::string_view tag;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;

	public:
#pragma message("TODO: most efficient way to copy in the members list")
		EnumType(StorageClass storage, TypeQualifier qualifier, std::optional<std::vector<EnumMember>> members, std::string_view tag)
			: storage{ storage }, qualifier{ qualifier }, members{ std::move(members) }, tag{ tag }
		{
		}
	};
}

#endif
