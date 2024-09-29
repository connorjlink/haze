#ifndef HAZE_COMMANDLINEPARSER_H
#define HAZE_COMMANDLINEPARSER_H

// Haze CommandLineParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CommandLineParser
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
