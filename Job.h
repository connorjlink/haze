#ifndef HAZE_JOB_H
#define HAZE_JOB_H

#include <string>
#include <chrono>

#include <cstdint>

// Haze Job.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Job
	{
	public:
		std::uint32_t _uuid;

	private:
		std::string _task;

	private:
		std::chrono::high_resolution_clock::time_point _start_time, _stop_time;

	private:
		std::chrono::microseconds duration() const;

	public:
		void end();

	public:
		std::string format() const;

	public:
		Job(std::string task, std::uint32_t uuid)
			: _task{ std::move(task) }, _uuid(uuid)
		{
			_start_time = std::chrono::high_resolution_clock::now();
			_stop_time = std::chrono::high_resolution_clock::time_point::min();
		}
	};
}

#endif
