#ifndef HAZE_COMMANDLINEPARSER_H
#define HAZE_COMMANDLINEPARSER_H

#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze CommandLineParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CommandLineParser
		: public InjectSingleton<ErrorReporter, CommandLineOptions>
	{
	private:
		std::vector<std::string> _filepaths;

	public:
		const decltype(_filepaths)& files() const
		{
			return _filepaths;
		}

	public:
		CommandLineParser()
		{
		}

	public:
		void parse(int, char**);
	};
}

#endif
