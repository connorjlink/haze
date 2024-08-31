#include "Job.h"
#include "ErrorReporter.h"

#include <format>

// Haze Job.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::chrono::microseconds Job::duration() const
	{
		if (_stop_time != std::chrono::high_resolution_clock::time_point::min())
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(_stop_time - _start_time);
		}

		_error_reporter->post_error("undefined job duration stop time", NULL_TOKEN);
		return std::chrono::microseconds{ 0 };
	}

	void Job::end()
	{
		_stop_time = std::chrono::high_resolution_clock::now();
	}

	std::string Job::format() const
	{
		return std::format("{} completed in {}", _task, duration());
	}
}
