import std;

#include <data/Tracking.h>

// Haze Tracking.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	TrackingTimePoint system_timestamp(void)
	{
		return std::chrono::system_clock::now();
	}
}
