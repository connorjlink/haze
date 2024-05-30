#ifndef HAZE_UTILITY_H
#define HAZE_UTILITY_H

#include <array>
#include <utility>
#include <vector>
#include <cstdint>

#include "Constants.h"

namespace hz
{
	inline static constexpr auto OPTIMIZE_AST = true;
	inline static constexpr auto OPTIMIZE_LTO = true;

    inline static constexpr auto DEBUG = true;

	std::array<word, 3> extract(std::uint32_t bytes);
}

#endif
