#ifndef HAZE_OPTIMIZATIONFLAG_H
#define HAZE_OPTIMIZATIONFLAG_H

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
	constexpr std::string_view to_string(OptimizationFlag flag)
	{
		switch (flag)
		{
#define X(enumerator, name, value) case OptimizationFlag::enumerator: return name;
#include <cli/OptimizationFlag.def>
#undef X
		}

		return "<unknown optimization flag>";
	}
}

template<>
struct std::formatter<hz::OptimizationFlag>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}
	auto format(const hz::OptimizationFlag& flag, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", to_string(flag));
	}
};

#endif 
