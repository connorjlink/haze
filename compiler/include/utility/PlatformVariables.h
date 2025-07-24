#ifndef HAZE_PLATFORMVARIABLES_H
#define HAZE_PLATFORMVARIABLES_H

// Haze PlatformVariables.h
// (c) Connor J. Link. All Rights Reserved.

//#define USE_64_BITS

namespace hz
{
#ifdef USE_64_BITS
	using native_uint = std::uint64_t;
	using native_int = std::int64_t;
#else
	using native_uint = std::uint32_t;
	using native_int = std::int32_t;
#endif

	enum : native_uint
	{
		NATIVE_UMAX = std::numeric_limits<native_uint>::max(),
		NATIVE_MAX = std::numeric_limits<native_int>::max(),
	};
}

#endif
