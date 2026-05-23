#ifndef HAZE_OPTIMIZATIONFLAG_H
#define HAZE_OPTIMIZATIONFLAG_H

#include <utility/Formatter.h>

// Haze OptimizationFlag.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class OptimizationFlag
	{
#define X(enumerator, name, value) enumerator = value,
#include <cli/OptimizationFlag.def>
#undef X
	};

	// NOTE: the following only works for a single flag
	constexpr std::string_view format_optimization_flag(OptimizationFlag flag)
	{
		switch (flag)
		{
#define X(enumerator, name, value) case OptimizationFlag::enumerator: return name;
#include <cli/OptimizationFlag.def>
#undef X
		}

		return "<unknown optimization flag>";
	}

	constexpr std::string to_string(OptimizationFlag flag)
	{
		static constexpr OptimizationFlag flags[] =
		{
#define X(enumerator, name, value) { OptimizationFlag::enumerator },
#include <cli/OptimizationFlag.def>
#undef X
		};

		auto result = std::string{};

		for (auto i = 0uz; i < std::size(flags); i++)
		{
			if (flag & flags[i])
			{
				result += format_optimization_flag(flags[i]);
				if (i != std::size(flags) - 1)
				{
					result += " | ";
				}
			}
		}

		return result;
	}
}

#endif 
