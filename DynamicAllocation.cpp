#include "DynamicAllocation.h"
#include "Log.h"

#include <fmt/format.h>

namespace hz
{
    Allocation::Type DynamicAllocation::atype()
    {
        return Allocation::Type::DYNAMIC;
    }

    Allocation* DynamicAllocation::allocate()
    {

    }

    void DynamicAllocation::deallocate()
    {

    }

    Register& DynamicAllocation::read()
    {
        Log::error(fmt::format("dynamic allocation at address {} cannot be mapped as a register", address));
    }

    void DynamicAllocation::write(std::uint8_t value)
    {
        
    }
}