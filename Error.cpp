import std;

#include "Error.h"

// Haze Error.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string Error::format() const
	{
		if (_position._line != 0 && _position._column != 0 && _erroring_line != "")
		{
			return std::format("  {} at {}:{}\n     >  {}  <\n      {}\n",
				_error_type_map.at(_severity), _position._line, _position._column, _erroring_line, _message);
		}

		return std::format("  {}: {}\n",
			_error_type_map.at(_severity), _message);
	}
}