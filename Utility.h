#ifndef HAZE_UTILITY_H
#define HAZE_UTILITY_H

#include <cstdint>
#include <array>

#include "Constants.h"

namespace hz
{
	std::array<word, 3> extract(std::uint32_t bytes);
}

#endif
