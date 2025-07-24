#ifndef HAZE_BINARYUTILITIES_H
#define HAZE_BINARYUTILITIES_H

#include <utility/Constants.h>

// Haze BinaryUtilities.h
// (c) Connor J. Link. All Rights Reserved.

#define PUT(x) out.append_range(x)

namespace hz
{
	constexpr byterange range8(std::uint8_t);
	constexpr byterange range16(std::uint16_t);
	constexpr byterange range32(std::uint32_t);
	constexpr byterange range64(std::uint64_t);
	byterange range_of(const std::string&);

	inline static const auto pad8 = range8(0);
	inline static const auto pad16 = range16(0);
	inline static const auto pad32 = range32(0);
	inline static const auto pad64 = range64(0);
}

#endif 
