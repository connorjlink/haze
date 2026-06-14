import std;

#include <utility/Random.h>

// Haze Random.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::size_t generate_digit_string(std::uint8_t digit_count)
	{
		static auto device = std::random_device{};
		static auto engine = std::mt19937_64{ device() };

		auto lower = 1uz;
		for (auto i = 0uz; i < digit_count; i++)
		{
			lower *= 10;
		}
		const auto upper = (lower * 10uz) - 1uz;

		auto distribution = std::uniform_int_distribution<std::size_t>{ lower, upper };
		return distribution(engine);
	}
}
