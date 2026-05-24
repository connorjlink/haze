#ifndef HAZE_VERBOSITYKIND_H
#define HAZE_VERBOSITYKIND_H

// Haze VerbosityKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class VerbosityKind
	{
#define X(enumerator, name) enumerator,
#include <cli/defs/VerbosityKind.x>
#undef X
	};

	constexpr std::string_view to_string(VerbosityKind verbosity)
	{
		switch (verbosity)
		{
#define X(enumerator, name) case VerbosityKind::enumerator: return #name;
#include <cli/defs/VerbosityKind.x>
#undef X
		}

		return "<unknown verbosity level>";
	}
}

#endif
