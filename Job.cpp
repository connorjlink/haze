#include "Job.h"
#include "Log.h"

#include <format>

namespace hz
{
	std::chrono::microseconds Job::duration() const
	{
		if (_stop_time != std::chrono::high_resolution_clock::time_point::min())
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(_stop_time - _start_time);
		}

		Log::error("undefined job duration stop time");
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
