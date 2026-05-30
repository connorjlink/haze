#ifndef HAZE_ERRORKIND_H
#define HAZE_ERRORKIND_H

// Haze ErrorKind.h
// (c) Connor J. Link. All Rights Reserved.

// weird Windows.h stuff... again...
#undef ERROR

namespace hz
{
	enum class ErrorKind
	{
#define X(enumerator, name) enumerator,
#include <error/defs/ErrorKind.x>
#undef X
	};

	constexpr std::string_view to_string(ErrorKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case ErrorKind::enumerator: return #name;
#include <error/defs/ErrorKind.x>
#undef X
		}

		return "<unknon error kind>";
	}
}

#endif
