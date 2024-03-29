#include "Allocator.h"
#include "Log.h"

#include <optional>
#include <fmt/format.h>

namespace
{
    std::optional<hz::Register> find_register(std::array<hz::Status, 4>& registers)
    {
        for (auto i = 0; i < registers.size(); i++)
        {
            if (registers[i] == hz::Status::FREE)
            {
                return { static_cast<hz::Register>(i) };
            }
        }

        return std::nullopt;
    }
}

namespace hz
{
    void Allocator::load(Register reg, int address)
    {
        generator->load(reg, address);
    }

    void Allocator::save(int address, Register reg)
    {
        generator->save(address, reg);
    }

    const Register& Allocator::read(Allocation& allocation)
    {
        if (allocation.type == AllocationType::STATIC)
        {
            return allocation.as.static_allocation->reg;
        }

        Log::error("only static allocations can be read directly");
    }

    void Allocator::write(Allocation& alloc, std::uint8_t value)
    {
        using enum AllocationType;
        switch (alloc.type)
        {
            case STATIC:
            {
                auto allocation = alloc.as.static_allocation;
                generator->copy(allocation->reg, value);
            } break;

            case DYNAMIC:
            {
                auto allocation = alloc.as.dynamic_allocation;

                auto store = [&]()
                {
                    generator->copy(R3, value);
                    generator->save(allocation->address, R3);
                };

                if (register_ledger[R3] == Status::FREE)
                {
                    store();
                }

                else
                {
                    generator->push(R3);
                    store();
                    generator->pull(R3);
                }

            } break;
        }
    }

    Allocation Allocator::allocate_static()
    {
        const auto reg = find_register(register_ledger);

        if (reg.has_value())
        {
            register_ledger[reg.value()] = Status::USED;
            return { .as = new StaticAllocation{ reg.value(), { false, {} } } };
        }

        //TODO: fix this in case other register was R3
        //caller may want to exclude a register from selection
        auto mem = allocate_dynamic(false);
        write(mem, R3);

        return { .as.static_allocation = new StaticAllocation{ R3, { true, mem } } };
    }

    Allocation Allocator::allocate_static(Register reg)
    {
        if (register_ledger[reg] == Status::FREE)
        {
            register_ledger[reg] = Status::USED;
            return { .as.static_allocation = new StaticAllocation{ reg, { false, {} } } };
        }

        //TODO: change this; logic doesn't work if caller relies on the location of storage
        return allocate_static();
    }

    void Allocator::deallocate_static(Allocation allocation)
    {
        auto static_allocation = allocation.as.static_allocation;
        register_ledger[static_allocation->reg] = Status::FREE;
    }

    Allocation Allocator::allocate_dynamic(bool free)
    {
        for (auto candidate = 0; candidate < heap_ledger.size(); candidate++)
        {
            if (heap_ledger[candidate] == Status::FREE)
            {
                return { .type = AllocationType::DYNAMIC, .as.dynamic_allocation = new DynamicAllocation{ static_cast<std::uint16_t>(candidate), free } };
            }
        }

        Log::error("heap region has region its maximum size");
    }

    [[maybe_unused]]
    Allocation Allocator::allocate_dynamic(std::uint16_t bytes, bool free)
    {
        for (auto address = 0; address < heap_ledger.size(); address++)
        {
            bool available = true;
            for (auto candidate = 0; candidate < bytes; candidate++)
            {
                if (heap_ledger[candidate] != Status::FREE)
                {
                    available = false;
                }
            }

            if (available)
            {
                return { .type = AllocationType::DYNAMIC, .as.dynamic_allocation = new DynamicAllocation{ static_cast<std::uint16_t>(address), free } };
            }
        }

        Log::error(fmt::format("could not allocate {} contiguous bytes on the available heap", bytes));
    }

    void Allocator::deallocate_dynamic(Allocation allocation)
    {
        auto dynamic_allocation = allocation.as.dynamic_allocation;
        if (dynamic_allocation->free)
        {
            heap_ledger[allocation.as.dynamic_allocation->address] = Status::FREE;
        }
    }

    void Allocator::deallocate(Allocation allocation)
    {
        using enum AllocationType;
        switch (allocation.type)
        {
            case STATIC:
            {
                deallocate_static(allocation);
            } break;

            case DYNAMIC:
            {
                deallocate_dynamic(allocation);
            } break;
        }
    }


    StaticAllocation::StaticAllocation(Register reg, State state)
        : reg(reg), state(state)
    {
    }

    StaticAllocation::~StaticAllocation()
    {
        if (state.need_restore)
        {
            Allocator::load(reg, state.restoree.as.dynamic_allocation->address);
        }

        Allocator::deallocate(Allocation{ .type = AllocationType::STATIC, .as.static_allocation = this });
    }

    DynamicAllocation::DynamicAllocation(std::uint16_t address, bool free)
        : address(address), free(free)
    {
    }

    DynamicAllocation::~DynamicAllocation()
    {
        Allocator::deallocate(Allocation{ .type = AllocationType::DYNAMIC, .as.dynamic_allocation = this });
    }
}