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

	void Log::print(std::string_view context, std::string_view message)
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
		print(to_string(severity), message);
	}
	
	void Log::output(const std::string& message)
	{
		print(to_string(ErrorKind::OUTPUT), message);
	}

	void Log::info(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::VERBOSE)
		{
			print(to_string(ErrorKind::INFORMATION), message);
		}
	}

	void Log::warning(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::VERBOSE ||
			USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::NORMAL)
		{
			print(to_string(ErrorKind::WARNING), message);
		}
	}

	void Log::error(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::VERBOSE ||
			USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::NORMAL ||
			USE_UNSAFE(CommandLineOptions)->verbosity == VerbosityKind::QUIET)
		{
			print(to_string(ErrorKind::ERROR), message);
		}
	}

	void Log::uncorrectable(const std::string& message)
	{
		if (USE_UNSAFE(CommandLineOptions)->verbosity != VerbosityKind::SILENT)
		{
			print(to_string(ErrorKind::UNCORRECTABLE), message);
		}
	}
}