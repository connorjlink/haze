#ifndef HAZE_OPTIMIZATIONFLAG_H
#define HAZE_OPTIMIZATIONFLAG_H

#include <utility/AutoEnum.h>
#include <utility/Formatter.h>

// Haze OptimizationFlag.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define OPTIMIZATION_FLAGS(X) \
	X(NONE, none,      0) \
	X(AST,  ast,  1 << 0) \
	X(TAC,  tac,  1 << 1) \
	X(LTO,  lto,  1 << 2)


#define ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define SWITCH_CASE(enumerator, name, value) case OptimizationFlag::enumerator: return #name;
#define MAP_MEMBER(enumerator, name, value) Mapping{ #name, OptimizationFlag::enumerator },
#define FORWARD_DECLARATION(enumerator, name, value) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, OPTIMIZATION_FLAGS, OptimizationFlag, optimization flag : std::uint8_t)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER


#pragma message("TODO: swap existing calls to std::format to this function for proper formatting of optimization flags")
	constexpr std::string format_optimization_flag(OptimizationFlag flag)
	{
		static constexpr OptimizationFlag flags[] =
		{
#define X(enumerator, name, value) { OptimizationFlag::enumerator },
			OPTIMIZATION_FLAGS(X)
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
