import std;

#include "RandomUtility.h"

// Haze RandomUtility.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	int generate(int num_digits)
	{
		auto distribution = std::uniform_int_distribution(0, (num_digits * 10) - 1);
		return distribution(_engine);
	}
}