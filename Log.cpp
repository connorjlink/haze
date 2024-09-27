import std;

#include "Log.h"
#include "CommandLineOptions.h"

// Haze Log.cpp
// (c) Connor J. Link. All Rights Reserved.

#define stream std::cerr

namespace hz
{
	void Log::print(const std::string& context, const std::string& message)
	{
		stream << std::format("{}: {}\n", context, message);
	}

	void Log::raw(const std::string& message)
	{
		stream << message;
	}

	void Log::with_severity(const std::string& message, ErrorType severity)
	{
		print(_error_type_map.at(severity), message);
	}
	
	void Log::output(const std::string& message)
	{
		print(_error_type_map.at(ErrorType::OUTPUT), message);
	}

	void Log::info(const std::string& message)
	{
		if (_options->_verbosity == VerbosityType::VERBOSE)
		{
			print(_error_type_map.at(ErrorType::INFORMATION), message);
		}
	}

	void Log::warning(const std::string& message)
	{
		if (_options->_verbosity == VerbosityType::VERBOSE ||
			_options->_verbosity == VerbosityType::NORMAL)
		{
			print(_error_type_map.at(ErrorType::WARNING), message);
		}
	}

	void Log::error(const std::string& message)
	{
		if (_options->_verbosity == VerbosityType::VERBOSE ||
			_options->_verbosity == VerbosityType::NORMAL ||
			_options->_verbosity == VerbosityType::QUIET)
		{
			print(_error_type_map.at(ErrorType::ERROR), message);
		}
	}

	void Log::uncorrectable(const std::string& message)
	{
		print(_error_type_map.at(ErrorType::UNCORRECTABLE), message);
	}
}