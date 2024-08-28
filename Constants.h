#ifndef HAZE_CONSTANTS_H
#define HAZE_CONSTANTS_H

#include <cstdint>
#include <limits>

#undef max

namespace hz
{
	using byte = std::uint8_t;
	using word = std::uint32_t;
	using byte = byte;

	inline static constexpr auto WORD_MAX = 0xFF + 1;
	inline static constexpr auto DWORD_MAX = 0xFFFF + 1;
	inline static constexpr auto HALF_WORD_MAX = WORD_MAX / 2;
	inline static constexpr auto HALF_DWORD_MAX = DWORD_MAX / 2;
	inline static constexpr auto QUARTER_WORD_MAX = HALF_WORD_MAX / 2;
	inline static constexpr auto QUARTER_DWORD_MAX = HALF_DWORD_MAX / 2;
}

#endif //HAZE_CONSTANTS_H
