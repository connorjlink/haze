#ifndef HAZE_STATISTIC_H
#define HAZE_STATISTIC_H

#include <utility/PlatformVariables.h>

// Haze Statistic.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#warning TODO ADD A STATISTIC EXPORTER CLASS

	struct TimedTask
	{
		std::string task;
		native_int microseconds;
	};

	struct InternalStatistic
	{
		native_int memory_allocated;
		native_int memory_freed;

		native_int tracked_entity_instantiations;
		native_int tracked_entity_retirements;
		native_int tracked_entity_destructions;

		native_int functions_count;
		native_int statements_count;
		native_int expressions_count;
		native_int symbols_count;

		std::vector<TimedTask> elapsed;
	};

	class Statistic
	{
		std::string stage;
		std::string filepath;

		InternalStatistic internal;
	};
}

#endif
