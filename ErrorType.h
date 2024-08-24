#ifndef HAZE_ERRORTYPE_H
#define HAZE_ERRORTYPE_H

#include <unordered_map>
#include <string>

namespace hz
{
	enum class ErrorType
	{
		OUTPUT,
		INFORMATION,
		WARNING,
		ERROR,
		UNCORRECTABLE,
	};

	static const std::unordered_map<ErrorType, std::string> _severity_map
	{
		{ ErrorType::OUTPUT, "output" },
		{ ErrorType::INFORMATION, "information" },
		{ ErrorType::WARNING, "warning" },
		{ ErrorType::ERROR, "error" },
		{ ErrorType::UNCORRECTABLE, "uncorrectable internal error"},
	};
}

#endif
