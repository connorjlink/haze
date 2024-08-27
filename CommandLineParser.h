#ifndef HAZE_COMMANDLINEPARSER_H
#define HAZE_COMMANDLINEPARSER_H

#include "CommandLineOptions.h"

#include <string>
#include <vector>

namespace hz
{
	class CommandLineParser
	{
	private:
		std::vector<std::string> _filepaths;

	public:
		const std::vector<std::string>& files() const
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