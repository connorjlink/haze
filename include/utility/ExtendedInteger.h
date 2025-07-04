#ifndef HAZE_EXTENDED_INTEGER_H
#define HAZE_EXTENDED_INTEGER_H

// Haze ExtendedInteger.h
// (c) Connor J. Link. All Rights Reserved.

#define EI(x) ExtendedInteger{ std::abs(x), x >= 0 }

namespace hz
{
	class ExtendedInteger
	{
	public:
		std::uint64_t magnitude;
		bool sign; // true = positive, false = negative

	public:
		ExtendedInteger()
			: magnitude{ 0 }, sign{ true }
		{
		}

		ExtendedInteger(std::uint64_t magnitude, bool sign)
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
	};

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
