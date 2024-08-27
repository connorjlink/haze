#ifndef HAZE_CONSTANTS_H
#define HAZE_CONSTANTS_H

#include <cstdint>
#include <limits>

#undef max

namespace hz
{
	using byte = std::uint8_t;
	using word = std::uint16_t;
	using byte = byte;

	static constexpr auto WORD_MAX = std::numeric_limits<byte>::max() + 1;
	static constexpr auto DWORD_MAX = std::numeric_limits<word>::max() + 1;
	static constexpr auto HALF_WORD_MAX = WORD_MAX / 2;
	static constexpr auto HALF_DWORD_MAX = DWORD_MAX / 2;
	static constexpr auto QUARTER_WORD_MAX = HALF_WORD_MAX / 2;
	static constexpr auto QUARTER_DWORD_MAX = HALF_DWORD_MAX / 2;
}

#endif //HAZE_CONSTANTS_H
