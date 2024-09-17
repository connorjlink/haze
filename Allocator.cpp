#include "Allocator.h"
#include "Allocation.h"
#include "Log.h"

import std;

// Haze Allocator.cpp
// (c) Connor J. Link. All Rights Reserved.

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


	/*StaticAllocation* Allocator::allocate_static(Register exclude, bool force)
	{
		if (auto reg = find_register(exclude); reg.has_value())
		{
			register_ledger[reg.value()] = Status::USED;
			return new StaticAllocation{ reg.value(), false };	
		}

		if (!force)
		{
			Log::error(std::format("static allocation failed with exclusion register {}", _register_map.at(exclude)));
		}

		auto new_reg = static_cast<Register>((exclude + 1) % R3);
		_generator->make_push(new_reg);
		return new StaticAllocation{ new_reg, true };
	}

	DynamicAllocation* Allocator::allocate_dynamic(std::uint32_t bytes, bool free)
	{
		for (auto i = 0u; i < heap_ledger.size(); i++)
		{
			bool available = true;
			for (auto j = i; j < bytes; j++)
			{
				if (heap_ledger[j] != Status::FREE)
				{
					available = false;
				}
			}

			if (available)
			{
				for (auto j = i; j < i + bytes; j++)
				{
					heap_ledger[j] = Status::USED;
				}

				return new DynamicAllocation{ static_cast<std::uint32_t>(i), free };
			}
		}

		Log::error(std::format("could not allocate {} contiguous bytes on the available heap", bytes));
	}*/
}