import std;

#include <error/Error.h>

// Haze Error.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string Error::format() const
	{
		if (location.line != 0 && location.column != 0 && _erroring_line != "")
		{
			return std::format("  {} at {}:{}\n     >  {}  <\n      {}\n",
				_error_type_map.at(_severity), location.line, location.column, _erroring_line, message);
		}

		return std::format("  {}: {}\n",
			_error_type_map.at(_severity), message);
	}
}