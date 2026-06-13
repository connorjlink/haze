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


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, StructOrUnionKind)

	DEFINE_ENUM(STRUCT_OR_UNION_KINDS, StructOrUnionKind, struct or union kind)

#undef AUTOENUM_ROUTER


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
		TypeKind type_kind() const;
		Offset size() const;
		bool is_complete() const;
	};
}

#endif 
