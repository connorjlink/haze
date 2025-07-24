#ifndef HAZE_EXTENDED_INTEGER_H
#define HAZE_EXTENDED_INTEGER_H

#include <utility/PlatformVariables.h>

// Haze ExtendedInteger.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	inline consteval native_int native(std::integral auto x)
	{
		return native_int{ static_cast<native_int>(x) };
	}

	inline consteval native_uint unative(std::integral auto x)
	{
		return native_uint{ static_cast<native_uint>(x) };
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
#pragma message ("TODO: implement spaceship comparison for ExtendedInteger")
		friend bool operator==(ExtendedInteger, const ExtendedInteger&);
		friend bool operator!=(ExtendedInteger, const ExtendedInteger&);
		friend bool operator>(ExtendedInteger, const ExtendedInteger&);
		friend bool operator>=(ExtendedInteger, const ExtendedInteger&);
		friend bool operator<(ExtendedInteger, const ExtendedInteger&);
		friend bool operator<=(ExtendedInteger, const ExtendedInteger&);
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
