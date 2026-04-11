#ifndef HAZE_STRUCTORUNION_H
#define HAZE_STRUCTORUNION_H

#include <type/Type.h>
#include <utility/Constants.h>

// Haze StructOrUnion.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    enum class StructOrUnionType
    {
        STRUCT,
        UNION
    };

    static const std::unordered_map<StructOrUnionType, std::string_view> _struct_or_union_type_map
    {
        { StructOrUnionType::STRUCT, "struct" },
        { StructOrUnionType::UNION, "union" },
    };
    
    class StructOrUnion : public TypeBase
	{
    public:
        struct StructOrUnionMember
        {
            std::string name;
            SumHandle type;
            Offset offset;
        };
    
	public:
		StorageClass storage;
		TypeQualifier qualifier;
        StructOrUnionType struct_or_union_type;
        std::optional<std::vector<StructOrUnionMember>> members;
        std::string tag;

    public:
        TypeType ttype() const
        {
            return TypeType::STRUCT_OR_UNION;
        }

        Offset size() const
        {
            if (!members.has_value())
            {
                CommonErrors::invalid_struct_or_union_type(struct_or_union_type, NULL_TOKEN);
                return -1;
            }

            using enum StructOrUnionType;
            switch (struct_or_union_type)
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
                    CommonErrors::invalid_struct_or_union_type(struct_or_union_type, NULL_TOKEN);
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
