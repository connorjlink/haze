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


	void StaticAllocation::deallocate()
	{
		allocator->register_ledger[reg] = Status::FREE;
	}

	void DynamicAllocation::deallocate()
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
		auto store = [&]()
		{
			generator->copy(R3, value);
			generator->save(address, R3);
		};

		if (allocator->register_ledger[R3] == Status::FREE)
		{
			store();
		}

		else
		{
			generator->push(R3);
			store();
			generator->pull(R3);
		}
	}

	
	void StaticAllocation::copy(Allocation* allocation) const 
	{

	}

	void DynamicAllocation::copy(Allocation* allocation) const
	{

	}
}
