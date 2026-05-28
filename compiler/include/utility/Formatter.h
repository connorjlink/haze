#ifndef HAZE_FORMATTER_H
#define HAZE_FORMATTER_H

// Haze Formatter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	concept Formattable = std::is_scoped_enum_v<T> && requires(const T& t)
	{
		{ std::to_underlying(t) } -> std::convertible_to<std::underlying_type_t<T>>;
		{ to_string(t) } -> std::convertible_to<std::string_view>;
	};

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T operator|(T lhs, T rhs)
	{
		return static_cast<T>(
			static_cast<std::underlying_type_t<T>>(lhs) |
			static_cast<std::underlying_type_t<T>>(rhs));
	}

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T operator|=(T& lhs, T rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr bool operator&(T lhs, T rhs)
	{
		return static_cast<bool>(
			static_cast<std::underlying_type_t<T>>(lhs) &
			static_cast<std::underlying_type_t<T>>(rhs));
	}
}

template<hz::Formattable T>
struct std::formatter<T>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}
	auto format(const T& value, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", to_string(value));
	}
};

#endif
