#ifndef HAZE_OPTIONKIND_H
#define HAZE_OPTIONKIND_H

// Haze OptionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class OptionKind
	{
#define X(enumerator, name) enumerator,
#include <cli/defs/OptionKind.def>
#undef X
	};

	constexpr std::string_view to_string(OptionKind option)
	{
		switch (option)
		{
#define X(enumerator, name) case OptionKind::enumerator: return #name;
#include <cli/defs/OptionKind.def>
#undef X
		}

		return "<unknown option>";
	}
}

#endif 
