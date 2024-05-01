#include "StaticAllocation.h"

namespace hz
{
    Allocation::Type StaticAllocation::atype()
    {
        return Allocation::Type::STATIC;
    }

    Allocation* StaticAllocation::allocate()
    {

    }

    void StaticAllocation::deallocate()
    {

    }

    Register& StaticAllocation::read()
    {

    }

    void StaticAllocation::write(std::uint8_t value)
    {

    }
}