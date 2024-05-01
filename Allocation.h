#ifndef HAZE_ALLOCATION_H
#define HAZE_ALLOCATION_H

#include "Instruction.h"

#include <cstdint>

#define AS_STATIC(x) static_cast<StaticAllocation*>(x)
#define AS_DYNAMIC(x) static_cast<DynamicAllocation*>(x)

namespace hz
{
    class Allocation
    {
    protected:
        friend class Allocator;
        enum class Type
        {
            STATIC,
            DYNAMIC,
        };

    public:
        virtual Allocation::Type atype() = 0;

        virtual Allocation* allocate() = 0;
        virtual void deallocate() = 0;

        virtual Register& read() = 0;
        virtual void write(std::uint8_t) = 0;
    };
}

#endif //HAZE_ALLOCATION_H
