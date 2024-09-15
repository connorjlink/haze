#ifndef HAZE_ALLOCATIONTYPE_H
#define HAZE_ALLOCATIONTYPE_H

namespace hz
{
	enum class AllocationType
	{
		STACK,
		STACK_AUTO,

		HEAP,
		HEAP_AUTO,
	};
}

#endif
