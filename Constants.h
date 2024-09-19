#ifndef HAZE_CONSTANTS_H
#define HAZE_CONSTANTS_H

// Haze Constants.h
// (c) Connor J. Link. All Rights Reserved.

#undef max

namespace hz
{
	using byte = std::uint8_t;
	using byterange = std::vector<std::uint8_t>;

	using word = std::uint16_t;
	using dword = std::uint32_t;

	inline static constexpr auto WORD_MAX = 0xFF + 1;
	inline static constexpr auto DWORD_MAX = 0xFFFF + 1;
	inline static constexpr auto HALF_WORD_MAX = WORD_MAX / 2;
	inline static constexpr auto HALF_DWORD_MAX = DWORD_MAX / 2;
	inline static constexpr auto QUARTER_WORD_MAX = HALF_WORD_MAX / 2;
	inline static constexpr auto QUARTER_DWORD_MAX = HALF_DWORD_MAX / 2;

	namespace x86
	{
		inline static constexpr auto BYTE_MAX = std::numeric_limits<byte>::max();
		inline static constexpr auto WORD_MAX = std::numeric_limits<word>::max();
		inline static constexpr auto DWORD_MAX = std::numeric_limits<dword>::max();
	}
}

#endif //HAZE_CONSTANTS_H
