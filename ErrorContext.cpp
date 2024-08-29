#include "ErrorContext.h"

#include <format>

// Haze ErrorContext.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	hz::TokenPosition locate(const hz::Token& token)
	{
#pragma message("TODO: implement token offsets to find the line and column here")
		return { 0, 0 };
	}

	std::string at_line(std::string file, std::int32_t line)
	{
#pragma message("TODO: implement getting the contents of a line by index")
		return "";
	}
}

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

	void ErrorContext::post(ErrorType severity, const std::string& message, const std::string& file, Token token)
	{
		const auto pos = TokenPosition{ token.line, token.column };
		_errors.emplace_back(Error{ pos, token.value, message, severity });
	}
}