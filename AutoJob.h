#ifndef HAZE_AUTOJOB_H
#define HAZE_AUTOJOB_H

#include "JobManager.h"
#include "DependencyInjector.h"

// Haze AutoJob.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AutoJob : public InjectService<JobManager>
	{
	private:
		std::uint32_t _job_id;

	public:
		AutoJob(const std::string&) noexcept;
		~AutoJob() noexcept;
	};
}

#endif
