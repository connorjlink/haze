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
        static const Register& read(Allocation*);
        static void write(Allocation*, std::uint8_t);

    private:
        inline static std::array<Status, 4> register_ledger = {};
        inline static std::array<Status, 32768> heap_ledger = {};

    private:
        static void deallocate_static(Allocation*);
        static void deallocate_dynamic(Allocation*);

    public:
        static Allocation* allocate_static(Register = DC);

        static Allocation* allocate_dynamic(bool = true);
        static Allocation* allocate_dynamic(std::uint16_t, bool = true);

        static void deallocate(Allocation*);
    };
}

#endif //HAZE_ALLOCATOR_H
