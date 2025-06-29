import std;

#include "JobManager.h"
#include "FileManager.h"
#include "ErrorReporter.h"

// Haze JobManager.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::uint32_t JobManager::begin_job(const std::string& task)
	{
		static std::uint32_t uuid = 0;
		_jobs.emplace_back(Job{ task, uuid });
		return uuid++;
	}

	void JobManager::end_job(std::uint32_t uuid)
	{
		for (auto& job : _jobs)
		{
			if (job.uuid == uuid)
			{
				job.end();
				return;
			}
		}

		USE_SAFE(ErrorReporter).post_error(std::format("job id {} is undefined and cannot be stopped", uuid), NULL_TOKEN);
	}

	void JobManager::log() const
	{
		for (auto& job : _jobs)
		{
			USE_SAFE(ErrorReporter).post_information(job.format(), NULL_TOKEN);
		}
	}
}