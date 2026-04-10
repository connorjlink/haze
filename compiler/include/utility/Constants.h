#ifndef HAZE_CONSTANTS_H
#define HAZE_CONSTANTS_H

// Haze Constants.h
// (c) Connor J. Link. All Rights Reserved.

// prevent <Windows.h> interference with std::max
#undef max

namespace hz
{
	using ByteRange = std::vector<std::uint8_t>;

	using RegisterIndex = std::uint8_t;
	// pointer type
	using MemoryIndex = std::uint32_t;
	using StackIndex = std::int32_t;
}

#endif
