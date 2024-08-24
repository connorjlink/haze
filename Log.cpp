#include "Log.h"

#include <iostream>
#include <format>
#include <cstdlib>

#define stream std::cerr

namespace hz
{
	void Log::print(std::string_view context, std::string_view message)
	{
		stream << std::format("{}: {}\n", context, message);
	}

	void Log::raw(std::string_view message)
	{
		stream << message;
	}

	void Log::with_severity(std::string_view message, ErrorType severity)
	{
		print(_severity_map.at(severity), message);
	}
	
	void Log::output(std::string_view message)
	{
		print(_severity_map.at(ErrorType::OUTPUT), message);
	}

	void Log::info(std::string_view message)
	{
		print(_severity_map.at(ErrorType::INFORMATION), message);
	}

	void Log::error(std::string_view message)
	{
		print(_severity_map.at(ErrorType::ERROR), message);
	}

	void Log::warning(std::string_view message)
	{
		print(_severity_map.at(ErrorType::WARNING), message);
	}

	void Log::uncorrectable(std::string_view message)
	{
		print(_severity_map.at(ErrorType::UNCORRECTABLE), message);
	}
}