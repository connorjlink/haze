#ifndef HAZE_STRUCTORUNIONTYPE_H
#define HAZE_STRUCTORUNIONTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/StorageClass.h>
#include <type/defs/StructOrUnionKind.h>
#include <type/defs/TypeKind.h>
#include <type/defs/TypeQualifier.h>
#include <utility/AutoEnum.h>
#include <utility/Constants.h>

// Haze StructOrUnionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct StructOrUnionType : public TypeBase
	{
	public:
		struct Member
		{
			std::string_view name;
			TypeHandle type;
			Offset offset;
		};

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		StructOrUnionKind kind;
		std::optional<std::vector<Member>> members;
		std::string_view tag;

	public:
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;

	public:
		StructOrUnionType(StorageClass storage, TypeQualifier qualifier, StructOrUnionKind kind, std::optional<std::vector<Member>> members, std::string_view tag)
			: storage{ storage }, qualifier{ qualifier }, kind{ kind }, members{ members }, tag{ tag }
		{
		}
	};
}

#endif 
