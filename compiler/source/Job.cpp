import std;

#include <job/Job.h>

// Haze Job.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::chrono::microseconds Job::duration() const
	{
		if (stop_time != std::chrono::high_resolution_clock::time_point::min())
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
		}

		USE_UNSAFE(ErrorReporter)->post_error("undefined job duration stop time", NULL_TOKEN);
		return std::chrono::microseconds{ 0 };
	}

	void Job::end()
	{
		stop_time = std::chrono::high_resolution_clock::now();
	}

	std::string Job::format() const
	{
		return std::format("{} completed in {}", task, duration());
	}
}
