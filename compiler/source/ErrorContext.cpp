import std;

#include <error/ErrorContext.h>

// Haze ErrorContext.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string ErrorContext::format() const
	{
		std::string result{};

		for (auto& error : errors)
		{
			result += std::format("  while {}\n  {}", task, error.format());
		}

		return result;
	}

	std::size_t ErrorContext::error_count() const
	{
		return errors.size();
	}

	void ErrorContext::post(ErrorKind severity, const std::string& message, const std::string& filepath, const Token& token)
	{
		const auto location = SourceLocation{ filepath, token.location.position, token.location.line, token.location.column };
		errors.emplace_back(Error{ location, token.text, message, severity });
	}
}