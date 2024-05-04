#include "Allocation.h"

namespace hz
{
    Allocation::Type StaticAllocation::atype() const
    {
        return Allocation::Type::STATIC;
    }

    Allocation::Type DynamicAllocation::atype() const
    {
        return Allocation::Type::DYNAMIC;
    }

    //Log::error(fmt::format("dynamic allocation at address {} cannot be mapped as a register", address));

}