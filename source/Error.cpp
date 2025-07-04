import std;

#include <error/Error.h>

// Haze Error.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string Error::format() const
	{
		if (_location.line != 0 && _location.column != 0 && _erroring_line != "")
		{
			return std::format("  {} at {}:{}\n     >  {}  <\n      {}\n",
				_error_type_map.at(_severity), _location.line, _location.column, _erroring_line, _message);
		}

		return std::format("  {}: {}\n",
			_error_type_map.at(_severity), _message);
	}
}