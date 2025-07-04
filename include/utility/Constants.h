#ifndef HAZE_CONSTANTS_H
#define HAZE_CONSTANTS_H

// Haze Constants.h
// (c) Connor J. Link. All Rights Reserved.

// prevent <Windows.h> interference with std::max
#undef max

namespace hz
{
	using byterange = std::vector<std::uint8_t>;

	using ubyte = std::uint8_t;
	using uword = std::uint16_t;
	using udword = std::uint32_t;
	using uqword = std::uint64_t;

	using sbyte = std::int8_t;
	using sword = std::int16_t;
	using sdword = std::int32_t;
	using sqword = std::int64_t;

	enum Limit : std::uint64_t
	{
		UBYTE_MAX  = std::numeric_limits<ubyte>::max(),
		UWORD_MAX  = std::numeric_limits<uword>::max(),
		UDWORD_MAX = std::numeric_limits<udword>::max(),
		UQWORD_MAX = std::numeric_limits<uqword>::max(),

		SBYTE_MAX  = std::numeric_limits<sbyte>::max(),
		SWORD_MAX  = std::numeric_limits<sword>::max(),
		SDWORD_MAX = std::numeric_limits<sdword>::max(),
		SQWORD_MAX = std::numeric_limits<sqword>::max(),
	};
}

#endif
