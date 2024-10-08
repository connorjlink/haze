#ifndef HAZE_RANDOMUTILITY_H
#define HAZE_RANDOMUTILITY_H

// Haze RandomUtility.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	inline static std::random_device _device;
	inline static std::mt19937 _engine(_device());

	int generate(int = 3);
}

#endif
