import std;

#include "RuntimeAllocator.h"

// Haze RuntimeAllocator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// GLOBALS
	RuntimeAllocator* _runtime_allocator;
	// GLOBALS


	std::uint32_t RuntimeAllocator::allocate(std::uint32_t bytes)
	{
		return {};
	}

	void RuntimeAllocator::define_local(const std::string& name)
	{
	}

	void RuntimeAllocator::define_local(const std::string& name, register_t source)
	{
	}

	void RuntimeAllocator::read_local(register_t destination, const std::string& name)
	{
	}

}
