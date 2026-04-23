#ifndef HAZE_ARRAYTYPE_H
#define HAZE_ARRAYTYPE_H

#include <type/Type.h>

// Haze ArrayType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class ArrayType : public TypeBase
    {
    public:
        SumHandle element_type;
        std::optional<> length;

    public:
        TypeType ttype() const
        {
            return TypeType::ARRAY;
        }

        Offset size() const
        {
            if (!length.has_value())
            {
                return Pointer::size();
            }

            return element_type.size() * length.value();
        }

        std::string string() const
        {
            const auto length_string = length.has_value() ? std::to_string(length.value()) : "";

            return std::format("{} {}[{}]",
                _type_qualifier_map.at(qualifier),
                element_type->string(),
                length_string);
        }

        bool is_complete() const
        {
            // an array is incomplete if it doesn't have a length yet
            // this does not work for functions since they decay to pointers as parameters
            if (!length.has_value())
            {
                return false;
            }

            return element_type->is_complete();
        }
    };
}

#endif
