#ifndef HAZE_TYPEDEFNAME_H
#define HAZE_TYPEDEFNAME_H

#include <type/Type.h>

// Haze TypedefName.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class TypedefName : public TypeBase
    {
    public:
        SumHandle type;
        std::string name;

    public:
        TypeType ttype() const
        {
            return TypeType::TYPEDEF_NAME;
        }

        Offset size() const
        {
            return type.size();
        }

        bool is_complete() const
        {
            return type.is_complete();
        }
    };
}

#endif
