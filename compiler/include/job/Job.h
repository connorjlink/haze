#ifndef HAZE_JOB_H
#define HAZE_JOB_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze Job.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Job : public InjectSingleton<ErrorReporter>
	{
	public:
		std::uint32_t uuid;

	private:
		std::string _task;
		std::chrono::high_resolution_clock::time_point _start_time, _stop_time;

	private:
		std::chrono::microseconds duration() const;

	public:
		std::string format() const;
		void end();

	public:
		Job(const std::string& task, std::uint32_t uuid)
			: _task{ task }, uuid(uuid)
		{
			_start_time = std::chrono::high_resolution_clock::now();
			_stop_time = std::chrono::high_resolution_clock::time_point::min();
		}
	};
}

#endif
