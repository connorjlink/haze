#ifndef HAZE_STACKALLOCATOR_H
#define HAZE_STACKALLOCATOR_H

#include <toolchain/models/InstructionEncoding.h>
#include <data/DependencyInjector.h>

// Haze StackAllocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// thread-local service because each translation effectively has its own memory space during compilation
	class StackAllocator : public ServiceTag<StackAllocator>
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

	public:
		bool is_available(register_t allocation)
		{
			return _ledger.contains(allocation);
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
		std::unordered_set<register_t> _ledger;

	public:
		StackAllocator();
	};
}

#endif
