#ifndef HAZE_STATICALLOCATION_H
#define HAZE_STATICALLOCATION_H

#include "Allocation.h"
#include "Instruction.h"

namespace hz
{
    class StaticAllocation : public Allocation
    {
    protected:
        friend class Allocator;

    protected:
        Register reg;
        struct State
        {
            bool need_restore;
            Allocation* restoree;
        } state;

    public:
        virtual Allocation::Type atype() final override;

        virtual Allocation* allocate() final override;
        virtual void deallocate() final override;

        virtual Register& read() final override;
        virtual void write(std::uint8_t) final override;
    };
}

#endif //HAZE_STATICALLOCATION_H
