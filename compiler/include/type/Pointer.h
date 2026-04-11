#ifndef HAZE_POINTER_H
#define HAZE_POINTER_H

#include <type/Type.h>

// Haze Pointer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class Pointer : public TypeBase
    {
    public:
        SumHandle pointee;
        TypeQualifier qualifier;

    public:
        TypeType ttype() const
        {
            return TypeType::POINTER;
        }

        Offset size() const
        {
            return sizeof(Address);
        }

        bool is_complete() const
        {
            // pointer has a fixed size, so it's always complete to allow for things like void*
            return true;
        }
    };
}

#endif
