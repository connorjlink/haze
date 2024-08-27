#ifndef HAZE_PEBUILDER_H
#define HAZE_PEBUILDER_H

#include <string>
#include <vector>
#include <cstdint>

#include "Constants.h"

#define PUT(x) out.append_range(x)

namespace hz
{
	class PEBuilder
	{
	public:
		using bytes_t = std::vector<std::uint8_t>;

	public:
		static bytes_t make8(std::uint8_t);
		static bytes_t make16(std::uint16_t);
		static bytes_t make32(std::uint32_t);

	public:
		static bytes_t build();
	};

	inline static const auto pad8 = PEBuilder::make8(0x0);
	inline static const auto pad16 = PEBuilder::make16(0x0);
	inline static const auto pad32 = PEBuilder::make32(0x0);
}

#endif 
