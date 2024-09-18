#ifndef HAZE_RUNTIMEALLOCATOR_H
#define HAZE_RUNTIMEALLOCATOR_H

#include <cstdint>

// Haze RuntimeAllocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class RuntimeAllocator
	{
	public:
		std::uint32_t allocate(std::uint32_t bytes);
	};

	extern RuntimeAllocator* _runtime_allocator;
}

#endif 
