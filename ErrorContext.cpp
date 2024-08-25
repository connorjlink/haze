#include "ErrorContext.h"

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
		const auto pos = ::locate(token);
		const auto line = ::at_line(file, pos._line);
		_errors.emplace_back(Error{ pos, line, message, severity });
	}
}