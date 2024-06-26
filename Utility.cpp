#include "Utility.h"

namespace hz
{
	std::array<std::uint8_t, 3> extract(std::uint32_t bytes)
	{
		return
		{
			static_cast<std::uint8_t>((bytes & 0xFF0000) >> 16),
			static_cast<std::uint8_t>((bytes & 0xFF00) >> 8),
			static_cast<std::uint8_t>((bytes & 0xFF) >> 0),
		};
	}
}