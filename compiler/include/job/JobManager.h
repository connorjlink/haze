#ifndef HAZE_JOBMANAGER_H
#define HAZE_JOBMANAGER_H

#include <job/Job.h>

// Haze JobManager.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class JobManager
		: public ServiceTag<JobManager>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		std::vector<Job> _jobs;

	public:
		std::uint32_t begin_job(const std::string&);
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
