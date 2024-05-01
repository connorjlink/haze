#ifndef HAZE_DYNAMICALLOCATION_H
#define HAZE_DYNAMICALLOCATION_H

#include "Allocation.h"

#include <cstdint>

namespace hz
{
    class DynamicAllocation : public Allocation
    {
    protected:
        friend class Allocator;

    protected:
        std::uint16_t address;

    public:
        virtual Allocation::Type atype() final override;
        virtual Allocation* allocate() final override;
        virtual void deallocate() final override;

        virtual Register& read() final override;
        virtual void write(std::uint8_t) final override;
    };
}

#endif //HAZE_DYNAMICALLOCATION_H
