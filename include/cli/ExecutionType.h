#ifndef HAZE_EXECUTIONTYPE_H
#define HAZE_EXECUTIONTYPE_H

// Haze ExecutionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ExecutionType
	{
		VALIDATE,
		COMPILE,
	};

	static const std::unordered_map<std::string, ExecutionType> _execution_map
	{
		{ "validate", ExecutionType::VALIDATE },
		{ "compile", ExecutionType::COMPILE },
	};
}

#endif
