#ifndef HAZE_UTILITY_H
#define HAZE_UTILITY_H

#include <cstdint>
#include <array>

#include "Constants.h"

namespace hz
{
	inline static constexpr auto OPTIMIZE = true;
    inline static constexpr auto DEBUG = true;

	std::array<word, 3> extract(std::uint32_t bytes);
}

#endif
