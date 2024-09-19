#ifndef HAZE_ERRORTYPE_H
#define HAZE_ERRORTYPE_H

// Haze ErrorType.h
// (c) Connor J. Link. All Rights Reserved.

#undef ERROR

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

	static const std::unordered_map<ErrorType, std::string> _error_type_map
	{
		{ ErrorType::OUTPUT, "output" },
		{ ErrorType::INFORMATION, "information" },
		{ ErrorType::WARNING, "warning" },
		{ ErrorType::ERROR, "error" },
		{ ErrorType::UNCORRECTABLE, "uncorrectable internal error"},
	};
}

#endif
