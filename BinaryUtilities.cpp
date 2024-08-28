#include "BinaryUtilities.h"
#include "Constants.h"

namespace hz
{
	byterange BinaryUtilities::range8(std::uint8_t value)
	{
		const byte b0 = value;
		return { b0 };
	}

	byterange BinaryUtilities::range16(std::uint16_t value)
	{
		const byte b0 = (value & 0xFF) >> 0;
		const byte b1 = (value & 0xFF00) >> 8;
		return { b0, b1 };
	}

	byterange BinaryUtilities::range32(std::uint32_t value)
	{
		const byte b0 = (value & 0xFF) >> 0;
		const byte b1 = (value & 0xFF00) >> 8;
		const byte b2 = (value & 0xFF0000) >> 16;
		const byte b3 = (value & 0xFF000000) >> 24;
		return { b0, b1, b2, b3 };
	}
}