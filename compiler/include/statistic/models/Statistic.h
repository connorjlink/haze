#ifndef HAZE_STATISTIC_H
#define HAZE_STATISTIC_H

// Haze Statistic.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#warning TODO ADD A STATISTIC EXPORTER CLASS

	struct TimedTask
	{
		std::string task;
		Offset microseconds;
	};

	struct InternalStatistic
	{
		Offset memory_allocated;
		Offset memory_freed;

		Offset tracked_entity_instantiations;
		Offset tracked_entity_retirements;
		Offset tracked_entity_destructions;

		Offset functions_count;
		Offset statements_count;
		Offset expressions_count;
		Offset symbols_count;

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
