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
	X(UNION, union)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case StructOrUnionKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, STRUCT_OR_UNION_KINDS, StructOrUnionKind, "struct or union kind")

#undef SWITCH_CASE
#undef ENUM_MEMBER


	class StructOrUnionType : public TypeBase
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
		TypeKind type_kind() const
		{
			return TypeKind::STRUCT_OR_UNION;
		}

		Offset size() const
		{
			if (!members.has_value())
			{
				CommonErrors::invalid_struct_or_union_type(struct_or_union_kind, NULL_TOKEN);
				return -1;
			}

			using enum StructOrUnionKind;
			switch (struct_or_union_kind)
			{
				case STRUCT:
				{
					// don't know here what the alignment is, so struct size is just the largest member's offset + its size
					Offset running_count{};
					for (const auto& member : members.value())
					{
						const auto member_size = member.type.size();
						const auto candidate = member.offset + member_size;

						if (candidate > running_count)
						{
							running_count = candidate;
						}
					}

					return running_count;
				} break;

				case UNION:
				{
					// union size is just the size of its largest member
					Offset running_count{};
					for (const auto& member : members.value())
					{
						const auto member_size = member.type.size();

						if (member_size > running_count)
						{
							running_count = member_size;
						}
					}

					return running_count;
				} break;

				default:
				{
					CommonErrors::invalid_struct_or_union_type(struct_or_union_kind, NULL_TOKEN);
					return -1;
				} break;
			}
		}

		bool is_complete() const
		{
			return members.has_value();
		}
	};
}

#endif 
