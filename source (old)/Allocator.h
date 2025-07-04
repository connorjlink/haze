#ifndef HAZE_ALLOCATOR_H
#define HAZE_ALLOCATOR_H

#include "Constants.h"
#include "InstructionEncoding.h"

// Haze Allocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class Status
	{
		FREE,
		USED,
	};

	class StaticAllocation;
	class DynamicAllocation;

	class Allocator
	{
	public:
		std::array<Status, 4> register_ledger = {};
		std::array<Status, HALF_DWORD_MAX> heap_ledger = {};

	public:
		std::optional<Register> find_register(Register = DC) const;

	public:
		// `Register` parameter specifies exclusion
		//StaticAllocation* allocate_static(Register = DC, bool = false);

		//// `bool` parameter specifies whether the memory should be automatically freed at destruction
		//DynamicAllocation* allocate_dynamic(std::uint32_t = 1, bool = true);
	};
}

#endif
