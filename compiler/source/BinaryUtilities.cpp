import std;

#include <utility/BinaryUtilities.h>

// Haze BinaryUtilities.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	template<typename T>
	constexpr ByteRange range(T input)
	{
		static constexpr auto bitcount = std::numeric_limits<std::uint8_t>::digits;
		static constexpr auto bytecount = std::numeric_limits<T>::digits / bitcount;

		ByteRange out{};

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
	constexpr ByteRange range8(std::uint8_t value)
	{
		return ::range(value);
	}

	constexpr ByteRange range16(std::uint16_t value)
	{
		return ::range(value);
	}

	constexpr ByteRange range32(std::uint32_t value)
	{
		return ::range(value);
	}

	constexpr ByteRange range64(std::uint64_t value)
	{
		return ::range(value);
	}

	ByteRange range_of(const std::string& s)
	{
		ByteRange out{};

		for (auto c : s)
		{
			out.emplace_back(static_cast<std::uint8_t>(c));
		}

		// builtin \00 null terminator byte for constant-table strings
		out.emplace_back(0x00);

		return out;
	}
}
