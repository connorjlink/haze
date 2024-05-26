#include "Utility.h"

namespace hz
{
	std::array<std::uint8_t, 3> extract(std::uint32_t bytes)
	{
		return
		{
			bytes & 0xF >> 0,
			bytes & 0xF0 >> 8,
			bytes & 0xF00 >> 16
		};
	}
}