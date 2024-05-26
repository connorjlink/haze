#include "Allocator.h"
#include "Allocation.h"
#include "Log.h"

#include <format>

namespace hz
{
	std::optional<Register> Allocator::find_register(Register exclude) const
	{
		for (auto i = 0; i < register_ledger.size(); i++)
		{
			if (i != exclude && register_ledger[i] == Status::FREE)
			{
				return { static_cast<Register>(i) };
			}
		}

		return std::nullopt;
	}


	Allocation* Allocator::allocate_static(Register exclude)
	{
		if (auto reg = find_register(exclude); reg.has_value())
		{
			register_ledger[reg.value()] = Status::USED;
			return new StaticAllocation{ reg.value() };	
		}

		Log::error(std::format("Static allocation failed with exclusion register {}", unmap(exclude)));
	}


	Allocation* Allocator::allocate_dynamic(bool free)
	{
		for (auto candidate = 0; candidate < heap_ledger.size(); candidate++)
		{
			if (heap_ledger[candidate] == Status::FREE)
			{
				return new DynamicAllocation{ static_cast<std::uint16_t>(candidate), free };
			}
		}

		Log::error("heap region has region its maximum size");
	}

	[[maybe_unused]]
	Allocation* Allocator::allocate_dynamic(std::uint16_t bytes, bool free)
	{
		for (std::uint16_t address = 0; address < heap_ledger.size(); address++)
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
				return new DynamicAllocation{ address, free };
			}
		}

		Log::error(std::format("could not allocate {} contiguous bytes on the available heap", bytes));
	}
}