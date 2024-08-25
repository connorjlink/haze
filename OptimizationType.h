#ifndef HAZE_OPTIMIZATIONTYPE_H
#define HAZE_OPTIMIZATIONTYPE_H

#include <string>
#include <unordered_map>

#include <cstdint>

namespace hz
{
	enum OptimizationType : std::uint32_t
	{
		OPTIMIZE_AST,
		OPTIMIZE_LTO,
	};

	static const std::unordered_map<std::string, OptimizationType> _optimization_map
	{
		{ "ast", OptimizationType::OPTIMIZE_AST },
		{ "lto", OptimizationType::OPTIMIZE_LTO },
	};
}

#endif 
