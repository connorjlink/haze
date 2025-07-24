#ifndef HAZE_ALLOCATIONTYPE_H
#define HAZE_ALLOCATIONTYPE_H

// Haze AllocationType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class AllocationType
	{
		OBSERVER,

		STACK,
		STACK_AUTO,

		HEAP,
		HEAP_AUTO,
	};
}

#endif
