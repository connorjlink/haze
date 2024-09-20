#ifndef HAZE_RUNTIMEALLOCATOR_H
#define HAZE_RUNTIMEALLOCATOR_H

#include "InstructionEncoding.h"

// Haze RuntimeAllocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class RuntimeAllocator
	{
	public:
		std::uint32_t allocate(std::uint32_t bytes);

	public:
		void define_local(const std::string&);
		void define_local(const std::string&, register_t);

	public:
		void read_local(register_t, const std::string&);
	};

	extern RuntimeAllocator* _runtime_allocator;
}

#endif 