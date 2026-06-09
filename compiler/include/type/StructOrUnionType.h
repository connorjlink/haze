#ifndef HAZE_STRUCTORUNIONTYPE_H
#define HAZE_STRUCTORUNIONTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeKind.h>
#include <utility/AutoEnum.h>
#include <utility/Constants.h>

// Haze StructOrUnionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define STRUCT_OR_UNION_KINDS(X) \
	X(STRUCT, struct) \
	X(UNION,  union)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case StructOrUnionKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, StructOrUnionKind::enumerator },
#define FORWARD_DECLARATION(enumerator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, STRUCT_OR_UNION_KINDS, StructOrUnionKind, "struct or union kind")

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER


	struct StructOrUnionType : public TypeBase
	{
	public:
		struct Member
		{
			std::string name;
			TypeHandle type;
			Offset offset;
		};

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		StructOrUnionKind struct_or_union_kind;
		std::optional<std::vector<Member>> members;
		std::string tag;

	public:
		Offset size() const;
		bool is_complete() const;
	};
}

#endif 
