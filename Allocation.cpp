#include "Allocation.h"

#include "Allocator.h"
#include "Generator.h"
#include "Log.h"

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


	StaticAllocation::~StaticAllocation()
	{
		allocator->register_ledger[reg] = Status::FREE;
	}

	DynamicAllocation::~DynamicAllocation()
	{
		if (need_free)
		{
			allocator->heap_ledger[address] = Status::FREE;
		}
	}


	Register& StaticAllocation::read()
	{
		return reg;
	}

	Register& DynamicAllocation::read()
	{
		Log::error(std::format("dynamic allocation at address {} cannot be mapped as a register for reading", address));
	}


	void StaticAllocation::write(std::uint8_t value)
	{
		generator->copy(reg, value);
	}

	void DynamicAllocation::write(std::uint8_t value)
	{
		auto do_write = [&](auto reg)
		{
			generator->copy(reg, value);
			generator->save(address, reg);
		};

		auto reg = allocator->find_register();

		if (reg.has_value())
		{
			do_write(reg.value());
		}

		else
		{
			generator->push(R3);
			do_write(R3);
			generator->pull(R3);
		}
	}

	
	void StaticAllocation::copy(Allocation* allocation) const 
	{
		switch (allocation->atype())
		{
			case Type::STATIC: generator->move(reg, AS_STATIC_ALLOCATION(allocation)->reg); break;
			case Type::DYNAMIC: generator->load(reg, AS_DYNAMIC_ALLOCATION(allocation)->address); break;
		}
	}

	void DynamicAllocation::copy(Allocation* allocation) const
	{
		switch (allocation->atype())
		{
			case Type::STATIC: generator->save(address, AS_STATIC_ALLOCATION(allocation)->reg); break;

			case Type::DYNAMIC: 
			{
				auto do_copy = [&](auto reg)
				{
					generator->load(reg, AS_DYNAMIC_ALLOCATION(allocation)->address);
					generator->save(address, reg);
				};

				auto reg = allocator->find_register();

				if (reg.has_value())
				{
					do_copy(reg.value());
				}

				else
				{
					generator->push(R3);
					do_copy(R3);
					generator->pull(R3);
				}
			} break;
		}
	}
}
