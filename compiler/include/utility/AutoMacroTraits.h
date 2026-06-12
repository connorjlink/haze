#ifndef HAZE_AUTOMACROTRAITS_H
#define HAZE_AUTOMACROTRAITS_H

// Haze AutoMacroTraits.h
// (c) Connor J. Link. All Rights Reserved.

// NOTE: the below declaration is only a guide to avoid from_string being an overload only on return type
namespace hz
{
	template<typename T>
	struct IsAutoBitfield : std::false_type
	{
	};

	template<typename T>
	struct IsAutoEnum : std::false_type
	{
	};


	template<typename T>
		requires std::is_scoped_enum_v<T>&& IsAutoBitfield<T>::value
	constexpr T from_string(std::string_view) = delete;

	template<typename T>
		requires std::is_scoped_enum_v<T> && IsAutoEnum<T>::value
	constexpr std::optional<T> from_string(std::string_view) = delete;
}

#endif
