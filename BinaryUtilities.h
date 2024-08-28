#ifndef HAZE_BINARYUTILITIES_H
#define HAZE_BINARYUTILITIES_H

#include "Constants.h"

#include <cstdint>

namespace hz
{
	class BinaryUtilities
	{
	public:
		static byterange range8(std::uint8_t);
		static byterange range16(std::uint16_t);
		static byterange range32(std::uint32_t);
	};

	inline static const auto pad8 = BinaryUtilities::range8(0x0);
	inline static const auto pad16 = BinaryUtilities::range16(0x0);
	inline static const auto pad32 = BinaryUtilities::range32(0x0);
}

#endif 
