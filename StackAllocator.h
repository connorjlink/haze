#ifndef HAZE_REGISTERALLOCATOR_H
#define HAZE_REGISTERALLOCATOR_H

#include "InstructionEncoding.h"

#include <set>

// Haze RegisterAllocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class StackAllocator
	{
	public:
		register_t allocate()
		{
			auto allocation = find_register();
			_ledger.insert(allocation);
			return allocation;
		}

	public:
		void release(register_t allocation)
		{
			_ledger.erase(allocation);
		}

	private:
		register_t find_register() const
		{
			auto current = 0;

			while (_ledger.contains(current))
			{
				++current;
			}

			return current;
		}

	private:
		std::set<register_t> _ledger;

	public:
		StackAllocator();
	};

	extern StackAllocator* _stack_allocator;
}

#endif
