import std;

#include "RuntimeAllocator.h"
#include "Generator.h"

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
		// NOTE: intentionally leaving undefined state to show that it is currently unmapped
		// but want to indicate that the variable exists at least
		_locals[name] = -1;
	}

	void RuntimeAllocator::define_local(const std::string& name, register_t source)
	{
		if (!_locals.contains(name))
		{
			_locals[name] = source;
			return;
		}

		_locals.at(name) = source;
	}

	void RuntimeAllocator::destroy_local(const std::string& name)
	{
		_locals.erase(name);
	}

	void RuntimeAllocator::read_local(register_t destination, const std::string& name)
	{
		const auto source = _locals.at(name);
		_generator->make_copy(destination, source);
	}
}
