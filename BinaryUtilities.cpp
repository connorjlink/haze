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
	/*constexpr byterange bin::range8(std::uint8_t value)
	{
		const byte b0 = value;
		return { b0 };
	}*/

	/*constexpr byterange bin::range16(std::uint16_t value)
	{
		const byte b0 = (value & 0xFF) >> 0;
		const byte b1 = (value & 0xFF00) >> 8;
		return { b0, b1 };
	}*/

	/*constexpr byterange bin::range32(std::uint32_t value)
	{
		const byte b0 = (value & 0xFF) >> 0;
		const byte b1 = (value & 0xFF00) >> 8;
		const byte b2 = (value & 0xFF0000) >> 16;
		const byte b3 = (value & 0xFF000000) >> 24;
		return { b0, b1, b2, b3 };
	}*/

	constexpr byterange bin::range8(std::uint8_t value)
	{
		return ::range(value);
	}

	constexpr byterange bin::range16(std::uint16_t value)
	{
		return ::range(value);
	}

	constexpr byterange bin::range32(std::uint32_t value)
	{
		return ::range(value);
	}

	constexpr byterange bin::range64(std::uint64_t value)
	{
		return ::range(value);
	}

	constexpr byterange bin::range_string(const std::string& string)
	{
		byterange out{};

		for (auto c : string)
		{
			out.emplace_back(static_cast<std::uint8_t>(c));
		}

		// builtin \00 null terminator byte for constant-table strings
		out.emplace_back(0x00);

		return out;
	}
}