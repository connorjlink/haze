#ifndef HAZE_HEAPALLOCATOR_H
#define HAZE_HEAPALLOCATOR_H

#include <data/DependencyInjector.h>

namespace hz
{
	// thread-local service because each translation effectively has its own memory space during compilation
	class HeapAllocator : public ServiceTag<HeapAllocator>
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
}

#endif
