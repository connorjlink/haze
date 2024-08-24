#include "Error.h"

#include <format>

namespace hz
{
	std::string Error::format() const
	{
		return std::format("at {}:{}\n\tfrom {}\n\t\t{}",
			_position._line, _position._column, _erroring_line, _message);
	}
}