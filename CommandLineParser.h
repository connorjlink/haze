#ifndef HAZE_COMMANDLINEPARSER_H
#define HAZE_COMMANDLINEPARSER_H

#include "CommandLineOptions.h"

#include <string>

namespace hz
{
	class CommandLineParser
	{
	private:

	public:
		CommandLineParser()
		{
		}

	public:
		CommandLineOptions parse(int, char**);
	};
}

#endif
