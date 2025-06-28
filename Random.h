#ifndef HAZE_RANDOM_H
#define HAZE_RANDOM_H

// Haze Random.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	inline static std::random_device _device;
	inline static std::mt19937 _engine(_device());

	int generate(int = 3);
}

#endif
