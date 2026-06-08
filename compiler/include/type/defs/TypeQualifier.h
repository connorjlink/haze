#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

#include <toolchain/models/Token.h>
#include <utility/AutoEnum.h>
#include <utility/Formatter.h>

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

// weird Windows.h stuff... again...
#undef CONST

namespace hz
{
#define TYPE_QUALIFIERS(X) \
	X(NONE,     none,          0) \
	X(CONST,    const,    1 << 0) \
	X(VOLATILE, volatile, 1 << 1) \
	X(RESTRICT, restrict, 1 << 2)


#define ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define SWITCH_CASE(enumerator, name, value) case TypeQualifier::enumerator: return #name;
#define MAP_MEMBER(enumerator, name, value) Mapping{ #name, TypeQualifier::enumerator },
#define FORWARD_DECLARATION(enumerator, name, value) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, TYPE_QUALIFIERS, TypeQualifier, type qualifier)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER


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
