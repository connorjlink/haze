import std;

#include "AutoJob.h"

// Haze AutoJob.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	AutoJob::AutoJob(const std::string& task) noexcept
	{
		_job_id = _job_manager->begin_job(task);
	}

	AutoJob::~AutoJob() noexcept
	{
		_job_manager->end_job(_job_id);
	}
}
