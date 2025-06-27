#ifndef HAZE_OPTIMIZATIONTYPE_H
#define HAZE_OPTIMIZATIONTYPE_H

// Haze OptimizationType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum OptimizationType : std::uint32_t
	{
		AST = 0x1,
		LTO = 0x2,
	};

	static const std::unordered_map<std::string, OptimizationType> _optimization_map
	{
		{ "ast", OptimizationType::AST },
		{ "lto", OptimizationType::LTO },
	};
}

#endif 
