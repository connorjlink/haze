import std;

#include <job/AutoJob.h>

// Haze AutoJob.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	AutoJob::AutoJob(const std::string& task) noexcept
	{
		_job_id = REQUIRE_SAFE(JobManager)->begin_job(task);
	}

	AutoJob::~AutoJob() noexcept
	{
		REQUIRE_SAFE(JobManager)->end_job(_job_id);
	}
}
