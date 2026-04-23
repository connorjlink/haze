#ifndef HAZE_POINTERTYPE_H
#define HAZE_POINTERTYPE_H

#include <type/Type.h>

// Haze PointerType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class PointerType : public TypeBase
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
