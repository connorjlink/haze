#ifndef HAZE_JOBMANAGER_H
#define HAZE_JOBMANAGER_H

#include "Job.h"

#include <vector>

// Haze JobManager.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class JobManager
	{
	private:
		std::vector<Job> _jobs;

	public:
		std::uint32_t begin_job(std::string);
		void end_job(std::uint32_t);

	public:
		void log() const;

	public:
		JobManager()
			: _jobs{}
		{
		}
	};

	extern JobManager* _job_manager;
}

#endif
