import std;

#include "BinaryUtilities.h"

// Haze BinaryUtilities.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	template<typename T>
	constexpr byterange range(T input)
	{
		static constexpr auto bitcount = std::numeric_limits<std::uint8_t>::digits;
		static constexpr auto bytecount = std::numeric_limits<T>::digits / bitcount;

		byterange out{};

		T mask = 0xFF;

		for (auto i = 0; i < bytecount; i++)
		{
			const auto result = (input & mask) >> (bitcount * i);
			out.emplace_back(static_cast<std::uint8_t>(result));
			mask <<= bitcount;
		}

		return out;
	}
}

namespace hz
{
	namespace bin
	{
		constexpr byterange range8(std::uint8_t value)
		{
			return ::range(value);
		}

		constexpr byterange range16(std::uint16_t value)
		{
			return ::range(value);
		}

		constexpr byterange range32(std::uint32_t value)
		{
			return ::range(value);
		}

		constexpr byterange range64(std::uint64_t value)
		{
			return ::range(value);
		}

		constexpr byterange range_string(const std::string& s)
		{
			byterange out{};

			for (auto c : s)
			{
				out.emplace_back(static_cast<std::uint8_t>(c));
			}

			// builtin \00 null terminator byte for constant-table strings
			out.emplace_back(0x00);

			return out;
		}
	}
}
