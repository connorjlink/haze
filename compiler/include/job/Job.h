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
		std::string task;
		std::chrono::high_resolution_clock::time_point start_time, stop_time;

	private:
		std::chrono::microseconds duration() const;

	public:
		std::string format() const;
		void end();

	public:
		Job(const std::string& task, std::uint32_t uuid)
			: task{ task }, uuid(uuid)
		{
			start_time = std::chrono::high_resolution_clock::now();
			stop_time = std::chrono::high_resolution_clock::time_point::min();
		}
	};
}

#endif
