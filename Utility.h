#ifndef HAZE_UTILITY_H
#define HAZE_UTILITY_H

#include <cstdint>
#include <array>

#include "Constants.h"

namespace hz
{
	std::array<word, 3> extract(std::uint32_t bytes)
	{
		return 
		{ 
			bytes & 0xF >> 0,
			bytes & 0xF0 >> 8,
			bytes & 0xF00 >> 16
		};
	}
}

#endif
