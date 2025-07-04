#ifndef HAZE_PLATFORMVARIABLES_H
#define HAZE_PLATFORMVARIABLES_H

// Haze PlatformVariables.h
// (c) Connor J. Link. All Rights Reserved.

//#define USE_64_BITS

namespace hz
{
#ifdef USE_64_BITS
	using native_int = std::uint64_t;
#else
	using native_int = std::uint32_t;
#endif

	enum : native_int
	{
		NATIVE_MAX = std::numeric_limits<native_int>::max(),
	};
}

#endif
