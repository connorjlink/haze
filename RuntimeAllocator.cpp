import std;

#include "RuntimeAllocator.h"
#include "Generator.h"
#include "ErrorReporter.h"

// Haze RuntimeAllocator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace 
{
	using namespace hz;

	void already_defined_error(const std::string& name, Token token)
	{
		_error_reporter->post_error(std::format("variable `{}` was already defined in the current scope", name), token);
	}

	void not_defined_error(const std::string& name, Token token)
	{
		_error_reporter->post_error(std::format("variable `{}` was not defined in the current scope", name), token);
	}
}

namespace hz
{
	// GLOBALS
	RuntimeAllocator* _runtime_allocator;
	// GLOBALS


	std::uint32_t RuntimeAllocator::allocate(std::uint32_t bytes)
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
	}

	void RuntimeAllocator::define_local(const std::string& name)
	{
		const auto& current_function = _generator->current_function();
		
		if (_locals_offsets[current_function].contains(name))
		{
			::already_defined_error(name, NULL_TOKEN);
			return;
		}

		// NOTE: intentionally leaving `undefined` state to show that it is currently unmapped
		// but want to indicate that the variable exists at least
		_locals_offsets[_generator->current_function()][name] = -1;
	}

	void RuntimeAllocator::define_local(const std::string& name, register_t source)
	{
		const auto& current_function = _generator->current_function();

		if (_locals_offsets[current_function].contains(name))
		{
			::already_defined_error(name, NULL_TOKEN);
			return;
		}

#pragma message("TODO: replace this hack with a more efficient algorithm to find the new offset to use!")

		auto& map = _locals_offsets[current_function];

		auto lowest_offset = 0;

		for (auto& [key, value] : map)
		{
			if (value < lowest_offset)
			{
				lowest_offset = value;
			}
		}

#pragma message("TODO: compute the size of the previous element (so probably our variables should be in an ordered map")
		const auto previous_size = 4;

		const auto offset = lowest_offset - previous_size;

		_generator->stack_write(offset, source);
		_locals_offsets[current_function][name] = offset;
	}

	void RuntimeAllocator::attach_local(const std::string& name, std::int32_t offset)
	{
		const auto& current_function = _generator->current_function();

		if (_locals_offsets[current_function].contains(name))
		{
			::already_defined_error(name, NULL_TOKEN);
			return;
		}

		_locals_offsets[current_function][name] = offset;
	}

	void RuntimeAllocator::destroy_local(const std::string& name)
	{
		_locals_offsets.erase(name);
	}

	void RuntimeAllocator::read_local(register_t destination, const std::string& name)
	{
		const auto& current_function = _generator->current_function();

		if (!_locals_offsets[current_function].contains(name))
		{
			::not_defined_error(name, NULL_TOKEN);
			return;
		}

		const auto offset = _locals_offsets[current_function].at(name);
		_generator->stack_read(destination, offset);
	}
}
