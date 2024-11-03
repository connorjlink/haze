#ifndef HAZE_BINARYUTILITIES_H
#define HAZE_BINARYUTILITIES_H

#include "Constants.h"

// Haze BinaryUtilities.h
// (c) Connor J. Link. All Rights Reserved.

#define PUT(x) out.append_range(x)

namespace hz
{
	namespace bin
	{
		constexpr byterange range8(std::uint8_t);
		constexpr byterange range16(std::uint16_t);
		constexpr byterange range32(std::uint32_t);
		constexpr byterange range64(std::uint64_t);

		byterange range_string(const std::string&);
	}

	inline static const auto pad8 = bin::range8(0);
	inline static const auto pad16 = bin::range16(0);
	inline static const auto pad32 = bin::range32(0);
	inline static const auto pad64 = bin::range64(0);
}

#endif 
