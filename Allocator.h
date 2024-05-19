#ifndef HAZE_ALLOCATOR_H
#define HAZE_ALLOCATOR_H

#include <array>
#include <cstdint>

#include "Generator.h"

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
        void load(Register, std::uint16_t);
        void save(std::uint16_t, Register);

    public:
        std::array<Status, 4> register_ledger = {};
        std::array<Status, 32768> heap_ledger = {};

    public:
        Allocation* allocate_static(Register = DC);

        Allocation* allocate_dynamic(bool = true);
        Allocation* allocate_dynamic(std::uint16_t, bool = true);
    };

    extern Allocator* allocator;
}

#endif //HAZE_ALLOCATOR_H
