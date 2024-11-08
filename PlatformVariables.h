#ifndef HAZE_PLATFORMVARIABLES_H
#define HAZE_PLATFORMVARIABLES_H

// Haze PlatformVariables.h
// (c) Connor J. Link. All Rights Reserved.

//#define USE_64_BITS

namespace hz
{
#ifdef USE_64_BITS
	using platform_address_size = std::uint64_t;
#else
	using platform_address_size = std::uint32_t;
#endif
}

#endif
