#ifndef HAZE_FORMATTER_H
#define HAZE_FORMATTER_H

// Haze Formatter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	concept IsFormattableEnumeration = std::is_scoped_enum_v<T> and requires(const T& t)
	{
		{ std::to_underlying(t) } -> std::convertible_to<std::underlying_type_t<T>>;
		{ to_string(t) } -> std::convertible_to<std::string_view>;
	};

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T operator|(T left, T right)
	{
		return static_cast<T>(
			std::to_underlying(left) |
			std::to_underlying(right));
	}

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T operator|(T left, std::underlying_type_t<T> right)
	{
		return left | static_cast<T>(right);
	}


	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T operator|=(T& left, T right)
	{
		left = left | right;
		return left;
	}

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T operator|=(T& left, std::underlying_type_t<T> right)
	{
		left = left | right;
		return left;
	}


	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr bool operator&(T left, T right)
	{
		return static_cast<bool>(
			std::to_underlying(left) &
			std::to_underlying(right));
	}

	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr bool operator&(T left, std::underlying_type_t<T> right)
	{
		return left & static_cast<T>(right);
	}
}

template<hz::IsFormattableEnumeration T>
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
