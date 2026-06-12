import std;

#include <error/models/Error.h>

// Haze Error.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string Error::format() const
	{
		if (location.line != 0 && location.column != 0 && erroring_line != "")
		{
			return std::format("  {} at {}:{}\n     >  {}  <\n      {}\n",
				severity, location.line, location.column, erroring_line, message);
		}

		return std::format("  {}: {}\n",
			severity, message);
	}
}