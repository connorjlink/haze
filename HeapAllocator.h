#ifndef HAZE_HEAPALLOCATOR_H
#define HAZE_HEAPALLOCATOR_H

#include <cstdint>

namespace hz
{
	class HeapAllocator
	{
	public:
		std::uint32_t allocate(std::uint32_t bytes)
		{
			return 0;
		}

		void release(std::uint32_t address, std::uint32_t bytes)
		{
			
		}
	};

	extern HeapAllocator* _heap_allocator;
}

#endif
