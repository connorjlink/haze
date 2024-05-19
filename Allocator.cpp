#include "Allocator.h"
#include "Allocation.h"
#include "Log.h"

#include <optional>
#include <format>

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
	void Allocator::load(Register reg, std::uint16_t address)
	{
		generator->load(reg, address);
	}

	void Allocator::save(std::uint16_t address, Register reg)
	{
		generator->save(address, reg);
	}


	//TODO: figure out if this is the target or exclusion register or what
	Allocation* Allocator::allocate_static(Register exclude)
	{
		//TODO: full implemention here
		return nullptr;
	}


	Allocation* Allocator::allocate_dynamic(bool free)
	{
		for (std::uint16_t candidate = 0; candidate < heap_ledger.size(); candidate++)
		{
			if (heap_ledger[candidate] == Status::FREE)
			{
				return new DynamicAllocation{ candidate, free };
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