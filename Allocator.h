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

    class Allocator;

    class StaticAllocation;
    class DynamicAllocation;

    enum class AllocationType
    {
        STATIC,
        DYNAMIC,
    };
    struct Allocation
    {
        const AllocationType type;

        union
        {
            StaticAllocation* static_allocation;
            DynamicAllocation* dynamic_allocation;
        } const as;
    };

    class StaticAllocation
    {
    private:
        friend class Allocator;

    public:
        const Register reg;

    private:
        struct State
        {
            const bool need_restore;
            const Allocation restoree;
        } const state;

    public:
        StaticAllocation(Register, State);
        ~StaticAllocation();
    };

    class DynamicAllocation
    {
    private:
        friend class Allocator;

    public:
        const std::uint16_t address;
        bool free;

    public:
        explicit DynamicAllocation(std::uint16_t, bool);
        ~DynamicAllocation();
    };

    class Allocator
    {
    private:
        inline static Generator* generator = nullptr;

    public:
        static void set_generator(Generator* gen)
        {
            generator = gen;
        }

    public:
        static void load(Register, int);
        static void save(int, Register);

    public:
        static const Register& read(Allocation&);
        static void write(Allocation&, std::uint8_t);

    private:
        inline static std::array<Status, 4> register_ledger = {};
        inline static std::array<Status, 32768> heap_ledger = {};

    private:
        static void deallocate_static(Allocation);
        static void deallocate_dynamic(Allocation);

    public:
        static Allocation allocate_static();
        static Allocation allocate_static(Register);

        static Allocation allocate_dynamic(bool = true);
        static Allocation allocate_dynamic(std::uint16_t, bool = true);

        static void deallocate(Allocation);
    };
}

#endif //HAZE_ALLOCATOR_H
