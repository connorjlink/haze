#ifndef HAZE_BINARYUTILITIES_H
#define HAZE_BINARYUTILITIES_H

#include <utility/Constants.h>

// Haze BinaryUtilities.h
// (c) Connor J. Link. All Rights Reserved.

#define PUT(x) out.append_range(x)

namespace hz
{
	namespace internal
	{
		template<typename T>
		constexpr ByteRange range(T input)
		{
			static constexpr auto bitcount = std::numeric_limits<Byte>::digits;
			static constexpr auto bytecount = std::numeric_limits<T>::digits / bitcount;

			ByteRange out{};

			T mask = 0xFF;

			for (auto i = 0; i < bytecount; i++)
			{
				const auto result = (input & mask) >> (bitcount * i);
				out.emplace_back(static_cast<Byte>(result));
				mask <<= bitcount;
			}

			return out;
		}
	}

	constexpr ByteRange range8(std::uint8_t value)
	{
		return internal::range(value);
	}

	constexpr ByteRange range16(std::uint16_t value)
	{
		return internal::range(value);
	}

	constexpr ByteRange range32(std::uint32_t value)
	{
		return internal::range(value);
	}

	constexpr ByteRange range64(std::uint64_t value)
	{
		return internal::range(value);
	}

	constexpr ByteRange range_of(const std::string& string)
	{
		auto out = ByteRange{};

		for (auto character : string)
		{
			out.emplace_back(static_cast<Byte>(character));
		}

		// builtin \00 null terminator byte for constant-table strings
		out.emplace_back(0x00);

		return out;
	}

	inline constexpr auto pad8 = range8(0);
	inline constexpr auto pad16 = range16(0);
	inline constexpr auto pad32 = range32(0);
	inline constexpr auto pad64 = range64(0);
}

#endif 
