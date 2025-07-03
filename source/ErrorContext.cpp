import std;

#include "ErrorContext.h"

// Haze ErrorContext.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string ErrorContext::format() const
	{
		std::string result{};

		for (auto& error : _errors)
		{
			result += std::format("  while {}\n  {}", _task, error.format());
		}

		return result;
	}

	std::size_t ErrorContext::error_count() const
	{
		return _errors.size();
	}

	void ErrorContext::post(ErrorType severity, const std::string& message, const std::string& file, const Token& token)
	{
		const auto location = SourceLocation{ file, token.location.position, token.location.line, token.location.column };
		_errors.emplace_back(Error{ location, token.text, message, severity });
	}
}