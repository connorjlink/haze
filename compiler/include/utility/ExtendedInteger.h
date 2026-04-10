#ifndef HAZE_EXTENDED_INTEGER_H
#define HAZE_EXTENDED_INTEGER_H

#include <error/ErrorReporter.h>

// Haze ExtendedInteger.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	inline consteval Offset native(std::integral auto x)
	{
		return Offset{ static_cast<Offset>(x) };
	}

	inline consteval Address unative(std::integral auto x)
	{
		return Address{ static_cast<Address>(x) };
	}

	class ExtendedInteger
	{
	public:
		std::uintmax_t magnitude;
		bool sign; // true = positive, false = negative

	public:
		ExtendedInteger()
			: magnitude{ 0 }, sign{ true }
		{
		}

		ExtendedInteger(std::uintmax_t magnitude, bool sign)
			: magnitude{ magnitude }, sign{ sign }
		{
		}

	public:
		operator bool();

	public:
		ExtendedInteger& operator=(const ExtendedInteger&);

	public:
		ExtendedInteger& operator+=(const ExtendedInteger&);
		ExtendedInteger& operator-=(const ExtendedInteger&);
		ExtendedInteger& operator*=(const ExtendedInteger&);
		ExtendedInteger& operator/=(const ExtendedInteger&);
		ExtendedInteger& operator%=(const ExtendedInteger&);

		ExtendedInteger& operator&=(const ExtendedInteger&);
		ExtendedInteger& operator^=(const ExtendedInteger&);
		ExtendedInteger& operator|=(const ExtendedInteger&);

	public:
		friend ExtendedInteger operator+(ExtendedInteger, const ExtendedInteger&);
		friend ExtendedInteger operator-(ExtendedInteger, const ExtendedInteger&);
		friend ExtendedInteger operator*(ExtendedInteger, const ExtendedInteger&);
		friend ExtendedInteger operator/(ExtendedInteger, const ExtendedInteger&);
		friend ExtendedInteger operator%(ExtendedInteger, const ExtendedInteger&);
		
		friend ExtendedInteger operator&(ExtendedInteger, const ExtendedInteger&);
		friend ExtendedInteger operator^(ExtendedInteger, const ExtendedInteger&);
		friend ExtendedInteger operator|(ExtendedInteger, const ExtendedInteger&);
		
		friend ExtendedInteger operator~(const ExtendedInteger&);
	
		friend ExtendedInteger operator+(const ExtendedInteger&);
		friend ExtendedInteger operator-(const ExtendedInteger&);

	public:
		friend bool operator==(ExtendedInteger, const ExtendedInteger&);
		friend bool operator!=(ExtendedInteger, const ExtendedInteger&);
		friend bool operator>(ExtendedInteger, const ExtendedInteger&);
		friend bool operator>=(ExtendedInteger, const ExtendedInteger&);
		friend bool operator<(ExtendedInteger, const ExtendedInteger&);
		friend bool operator<=(ExtendedInteger, const ExtendedInteger&);

	public:
		template<typename T>
			requires std::integral<T>
		bool is_within_range() const
		{
			if constexpr (std::is_signed_v<T>)
			{
				if (sign)
				{
					// positive
					return magnitude <= static_cast<std::uintmax_t>(std::numeric_limits<T>::max());
				}

				return magnitude >= static_cast<std::uintmax_t>(-static_cast<std::intmax_t>(std::numeric_limits<T>::min()));
			}
			else
			{
				return magnitude <= static_cast<std::uintmax_t>(std::numeric_limits<T>::max());
			}
		}

		template<typename T>
			requires std::integral<T>
		T to_integral() const
		{
			if (!is_within_range<T>())
			{
				// internal compiler error: crash out
				USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
					"extended integral value `{}{}` out of range", sign ? "+" : "-", magnitude), NULL_TOKEN);
			}

			if constexpr (std::is_signed_v<T>)
			{
				if (sign)
				{
					return static_cast<T>(magnitude);
				}
				else
				{
					return static_cast<T>(-static_cast<std::intmax_t>(magnitude));
				}
			}
			else
			{
				return static_cast<T>(magnitude);
			}
		}

	public:
		std::string to_string() const
		{
			return std::format("{}{}", sign ? "+" : "-", magnitude);
		}
	};

	template<typename T>
	concept IsIntegralMax = std::is_same_v<T, std::intmax_t> or std::is_same_v<T, std::uintmax_t>;

	inline constexpr std::uintmax_t abs_integral_max(IsIntegralMax auto t)
	{
		if constexpr (std::is_signed_v<decltype(t)>)
		{
			return static_cast<std::uintmax_t>(t < 0 ? -t : t);
		}
		else
		{
			return static_cast<std::uintmax_t>(t);
		}
	}


	inline constexpr ExtendedInteger EI(IsIntegralMax auto t)
	{
		return ExtendedInteger{ abs_integral_max(t), t >= 0 };
	}

	/*inline constexpr ExtendedInteger operator""_ext(IsIntegralMax auto t)
	{
		return EI(t);
	}*/

	ExtendedInteger operator+(ExtendedInteger, const ExtendedInteger&);
	ExtendedInteger operator-(ExtendedInteger, const ExtendedInteger&);
	ExtendedInteger operator*(ExtendedInteger, const ExtendedInteger&);
	ExtendedInteger operator/(ExtendedInteger, const ExtendedInteger&);
	ExtendedInteger operator%(ExtendedInteger, const ExtendedInteger&);

	ExtendedInteger operator&(ExtendedInteger, const ExtendedInteger&);
	ExtendedInteger operator^(ExtendedInteger, const ExtendedInteger&);
	ExtendedInteger operator|(ExtendedInteger, const ExtendedInteger&);
	
	ExtendedInteger operator~(const ExtendedInteger&);
	
	ExtendedInteger operator+(const ExtendedInteger&);
	ExtendedInteger operator-(const ExtendedInteger&);

	bool operator==(ExtendedInteger, const ExtendedInteger&);
	bool operator!=(ExtendedInteger, const ExtendedInteger&);
	bool operator>(ExtendedInteger, const ExtendedInteger&);
	bool operator>=(ExtendedInteger, const ExtendedInteger&);
	bool operator<(ExtendedInteger, const ExtendedInteger&);
	bool operator<=(ExtendedInteger, const ExtendedInteger&);
}

#endif
