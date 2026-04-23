#ifndef HAZE_ENUMTYPE_H
#define HAZE_ENUMTYPE_H

#include <type/Type.h>
#include <utility/Constants.h>

// Haze EnumType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class EnumType : public TypeBase
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
        TypeType ttype() const
        {
            return TypeType::ENUM;
        }

        Offset size() const
        {
            // C (until C23?) doesn't support specifying the underlying type of enumerators
            return sizeof(EnumMember::value);
        }

        bool is_complete() const
        {
            return members.has_value();
        }
    };
}

#endif
