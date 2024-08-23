#include "JobManager.h"
#include "Log.h"

#include <format>

namespace hz
{
	std::uint32_t JobManager::begin_job(std::string task)
	{
		static std::uint32_t uuid = 0;
		_jobs.emplace_back(Job{ std::move(task), uuid });
		return uuid++;
	}

	void JobManager::end_job(std::uint32_t uuid)
	{
		for (auto& job : _jobs)
		{
			if (job._uuid == uuid)
			{
				job.end();
				return;
			}
		}

		Log::error(std::format("job id {} is undefined and cannot be stopped", uuid));
	}

	std::string JobManager::format() const
	{
		std::string result;

		for (auto& job : _jobs)
		{
			result += std::format("{}\n", job.format());
		}

		return result;
	}
}