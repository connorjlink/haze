#ifndef HAZE_EXECUTIONTYPE_H
#define HAZE_EXECUTIONTYPE_H

#include <string>
#include <unordered_map>

// Haze ExecutionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ExecutionType
	{
		COMPILE,
		SIMULATE,
	};

	static const std::unordered_map<std::string, ExecutionType> _execution_map
	{
		{ "compile", ExecutionType::COMPILE },
		{ "simulate", ExecutionType::SIMULATE },
	};
}

#endif
