#ifndef HAZE_UTILITY_H
#define HAZE_UTILITY_H

#include "Constants.h"

#include <array>
#include <cstdint>

#include <random>

namespace hz
{
	// abstract syntax tree optimizations
	inline static constexpr auto OPTIMIZE_AST = true;
	// peephole link-time optimizations
	inline static constexpr auto OPTIMIZE_LTO = true;

    inline static constexpr auto DEBUG = true;

	std::array<word, 3> extract(std::uint32_t bytes);

	inline static std::random_device _device;
	inline static std::mt19937 _engine(_device());

	int generate(int = 3);
}

#endif
