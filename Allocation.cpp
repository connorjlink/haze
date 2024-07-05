#include "Allocation.h"

#include "Allocator.h"
#include "Generator.h"
#include "IdentifierExpression.h"
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
		if (was_forced)
		{
			generator->make_pull(reg);
			return;
		}

		allocator->register_ledger[reg] = Status::FREE;
	}

	DynamicAllocation::~DynamicAllocation()
	{
		if (need_free)
		{
			allocator->heap_ledger[address] = Status::FREE;
		}
	}


	Register StaticAllocation::read()
	{
		return reg;
	}

	Register DynamicAllocation::read()
	{
		Log::error(std::format("dynamic allocation at address {} cannot be mapped as a register for reading", address));
	}


	void StaticAllocation::write(std::uint8_t value)
	{
		generator->make_copy(reg, value);
	}

	void DynamicAllocation::write(std::uint8_t value)
	{
		auto do_write = [&](auto reg)
		{
			generator->make_copy(reg, value);
			generator->make_save(address, reg);
		};

		auto reg = allocator->find_register();

		if (reg.has_value())
		{
			do_write(reg.value());
		}

		else
		{
			generator->make_push(R3);
			do_write(R3);
			generator->make_pull(R3);
		}
	}

	
	void StaticAllocation::copy(Allocation* allocation) const 
	{
		switch (allocation->atype())
		{
			case Type::STATIC: generator->make_move(reg, AS_STATIC_ALLOCATION(allocation)->reg); break;
			case Type::DYNAMIC: generator->make_load(reg, AS_DYNAMIC_ALLOCATION(allocation)->address); break;
		}
	}

	void DynamicAllocation::copy(Allocation* allocation) const
	{
		switch (allocation->atype())
		{
			case Type::STATIC: generator->make_save(address, AS_STATIC_ALLOCATION(allocation)->reg); break;

			case Type::DYNAMIC: 
			{
				auto do_copy = [&](auto reg)
				{
					generator->make_load(reg, AS_DYNAMIC_ALLOCATION(allocation)->address);
					generator->make_save(address, reg);
				};

				auto reg = allocator->find_register();

				if (reg.has_value())
				{
					do_copy(reg.value());
				}

				else
				{
					generator->make_push(R3);
					do_copy(R3);
					generator->make_pull(R3);
				}
			} break;
		}
	}



	ManagedStaticAllocation::ManagedStaticAllocation(Register reg, bool force)
	{
		allocation = allocator->allocate_static(reg, force);
	}

	ManagedStaticAllocation::~ManagedStaticAllocation()
	{
		delete allocation;
	}


	ManagedDynamicAllocation::ManagedDynamicAllocation(std::uint16_t bytes, bool free)
	{
		allocation = allocator->allocate_dynamic(bytes, free);
	}

	ManagedDynamicAllocation::~ManagedDynamicAllocation()
	{
		delete allocation;
	}
}
