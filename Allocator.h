#ifndef HAZE_ALLOCATOR_H
#define HAZE_ALLOCATOR_H

#include "Constants.h"
#include "Generator.h"

#include <array>
#include <cstdint>
#include <optional>

namespace hz
{
    enum class Status
    {
        FREE,
        USED,
    };

    class Allocator
    {
    public:
        std::array<Status, 4> register_ledger = {};
        std::array<Status, HALF_DWORD_MAX> heap_ledger = {};

    public:
        std::optional<Register> find_register(Register = DC) const;

    public:
        // `Register` parameter specifies exclusion
        Allocation* allocate_static(Register = DC, bool = false);

        // `bool` parameter specifies whether the memory should be automatically freed at destruction
        Allocation* allocate_dynamic(bool = true);
        Allocation* allocate_dynamic(std::uint16_t, bool = true);
    };

    extern Allocator* allocator;
}

#endif //HAZE_ALLOCATOR_H
