#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

#include <toolchain/models/Token.h>
#include <utility/Formatter.h>

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#undef CONST

	enum class TypeQualifier
	{
#define X(enumerator, name, value) name = value,
#include <type/defs/TypeQualifier.def>
#undef X
	};

	constexpr std::string_view format_type_qualifier(TypeQualifier qualifier)
	{
		using enum TypeQualifier;
		switch (qualifier)
		{
#define X(enumerator, name, value) case TypeQualifier::name: return #name;
#include <type/defs/TypeQualifier.def>
#undef X
		}

		return "<unknown type qualifier>";
	}

	constexpr std::string to_string(TypeQualifier qualifier)
	{
		static constexpr TypeQualifier qualifiers[] =
		{
#define X(enumerator, name, value) { TypeQualifier::name },
#include <type/defs/TypeQualifier.def>
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
