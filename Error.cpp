#include "Error.h"

#include <format>

namespace hz
{
	std::string Error::format() const
	{
		return std::format("at {}:{}\n\tfrom {}\n\t\t{}",
			_line, _col, _erroring_line, _message);
	}
}