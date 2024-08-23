#ifndef HAZE_JOBMANAGER_H
#define HAZE_JOBMANAGER_H

#include <vector>

#include "Job.h"

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
}

#endif
