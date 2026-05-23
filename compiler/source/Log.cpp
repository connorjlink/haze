import std;

#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <io/Log.h>

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
		if (USE_UNSAFE(CommandLineOptions)->verbosity != VerbosityKind::SILENT)
		{
			stream << message;
		}
	}

	void Log::with_severity(const std::string& message, ErrorKind severity)
	{
		print(_error_type_map.at(severity), message);
	}
	
	void Log::output(const std::string& message)
	{
		print(_error_type_map.at(ErrorKind::OUTPUT), message);
	}

	void Log::info(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::VERBOSE)
		{
			print(_error_type_map.at(ErrorKind::INFORMATION), message);
		}
	}

	void Log::warning(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::VERBOSE ||
			USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::NORMAL)
		{
			print(_error_type_map.at(ErrorKind::WARNING), message);
		}
	}

	void Log::error(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::VERBOSE ||
			USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::NORMAL ||
			USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::QUIET)
		{
			print(_error_type_map.at(ErrorKind::ERROR), message);
		}
	}

	void Log::uncorrectable(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity != VerbosityKind::SILENT)
		{
			print(_error_type_map.at(ErrorKind::UNCORRECTABLE), message);
		}
	}
}