#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeQualifier
	{
		IMMUTABLE,
		MUTABLE,
	};

	static const std::unordered_map<TypeQualifier, std::string> _type_qualifier_map
	{
		{ TypeQualifier::IMMUTABLE, "immutable" },
		{ TypeQualifier::MUTABLE, "mutable" },
	};
}

#endif