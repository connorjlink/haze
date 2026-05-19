#ifndef HAZE_STRUCTORUNIONTYPE_H
#define HAZE_STRUCTORUNIONTYPE_H

#include <error/CommonErrors.h>
#include <type/StorageClass.h>
#include <type/TypeQualifier.h>
#include <type/TypeKind.h>
#include <utility/Constants.h>

// Haze StructOrUnion.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class StructOrUnionKind
	{
		STRUCT,
		UNION
	};

	static const std::unordered_map<StructOrUnionKind, std::string_view> _struct_or_union_type_map
	{
		{ StructOrUnionKind::STRUCT, "struct" },
		{ StructOrUnionKind::UNION, "union" },
	};

	class StructOrUnionType : public TypeBase
	{
	public:
		struct StructOrUnionMember
		{
			std::string name;
			TypeHandle type;
			Offset offset;
		};

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		StructOrUnionKind struct_or_union_kind;
		std::optional<std::vector<StructOrUnionMember>> members;
		std::string tag;

	public:
		TypeKind ttype() const
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
						const auto member_size = member.type.get().size();
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
