#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

#include <toolchain/models/Token.h>
#include <utility/AutoEnum.h>
#include <utility/Formatter.h>

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#undef CONST

#include <type/defs/TypeQualifier.x>

#define ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define SWITCH_CASE(enumerator, name, value) case TypeQualifier::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, TYPE_QUALIFIERS, TypeQualifier, type qualifier)

	constexpr std::string format_type_qualifier(TypeQualifier qualifier)
	{
		static constexpr TypeQualifier qualifiers[] =
		{
#define X(enumerator, name, value) TypeQualifier::enumerator,
			TYPE_QUALIFIERS(X)
#undef X
		};

		auto result = std::string{};

		for (auto i = 0uz; i < std::size(qualifiers); i++)
		{
			if (qualifier & qualifiers[i])
			{
				result += format_type_qualifier(qualifiers[i]);
				if (i != std::size(qualifiers) - 1)
				{
					result += " | ";
				}
			}
		}

		return result;
	}
}

#endif
