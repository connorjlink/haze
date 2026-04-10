#ifndef HAZE_CONSTANTS_H
#define HAZE_CONSTANTS_H

// Haze Constants.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using ByteRange = std::vector<std::uint8_t>;

	using Register = std::uint8_t;
	// pointer type
	using Address = std::uint32_t;
	using Offset = std::int32_t;

	// with only 32-bit support for now, this is sufficient to hold any immediate value
	// if 64-bit support is ever added, this can be swapped out drop-in style with true bigint class type
	using BigInteger = std::int64_t;
}

#endif
