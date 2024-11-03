import std;

#include "ExtendedInteger.h"

// Haze ExtendedInteger.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExtendedInteger::operator bool()
	{
		return static_cast<bool>(magnitude);
	}

	ExtendedInteger& ExtendedInteger::operator=(const ExtendedInteger& rhs)
	{
		magnitude = rhs.magnitude;
		sign = rhs.sign;
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator+=(const ExtendedInteger& rhs)
	{
		if (sign == rhs.sign)
		{
			magnitude += rhs.magnitude;
		}

		else
		{
			if (magnitude >= rhs.magnitude)
			{
				magnitude -= rhs.magnitude;
			}

			else
			{
				magnitude = rhs.magnitude - magnitude;
				sign = rhs.sign;
			}
		}

		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator-=(const ExtendedInteger& rhs)
	{
		if (sign != rhs.sign) {
			magnitude += rhs.magnitude;
		}
		else {
			if (magnitude >= rhs.magnitude) {
				magnitude -= rhs.magnitude;
			}
			else {
				magnitude = rhs.magnitude - magnitude;
				sign = !sign;
			}
		}
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator*=(const ExtendedInteger& rhs)
	{
		magnitude *= rhs.magnitude;
		sign = (sign == rhs.sign);
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator/=(const ExtendedInteger& rhs)
	{
		magnitude /= rhs.magnitude;
		sign = (sign == rhs.sign);
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator%=(const ExtendedInteger& rhs)
	{
		magnitude %= rhs.magnitude;
		// C/C++ modulo by definition has the same sign as the lhs
		// for example, 5 % -3 = 2
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator&=(const ExtendedInteger& rhs)
	{
		magnitude &= rhs.magnitude;
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the lhs
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator^=(const ExtendedInteger& rhs)
	{
		magnitude ^= rhs.magnitude;
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the lhs
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator|=(const ExtendedInteger& rhs)
	{
		magnitude |= rhs.magnitude;
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the lhs
		return *this;
	}

	ExtendedInteger operator+(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs += rhs;
	}

	ExtendedInteger operator-(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs -= rhs;
	}

	ExtendedInteger operator*(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs *= rhs;
	}

	ExtendedInteger operator/(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs /= rhs;
	}

	ExtendedInteger operator%(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs %= rhs;
	}

	ExtendedInteger operator&(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs &= rhs;
	}

	ExtendedInteger operator^(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs ^= rhs;
	}

	ExtendedInteger operator|(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return lhs |= rhs;
	}

	ExtendedInteger operator~(const ExtendedInteger& rhs)
	{
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the rhs
		return { ~rhs.magnitude, rhs.sign };
	}

	ExtendedInteger operator+(const ExtendedInteger& rhs)
	{
		return rhs;
	}

	ExtendedInteger operator-(const ExtendedInteger& rhs)
	{
		return { rhs.magnitude, !rhs.sign };
	}

	bool operator==(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return (lhs.magnitude == rhs.magnitude) && (lhs.sign == rhs.sign);
	}

	bool operator!=(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator>(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		if (lhs.sign != rhs.sign)
		{
			return lhs.sign;
		}

		return lhs.sign ? (lhs.magnitude > rhs.magnitude) : (lhs.magnitude < rhs.magnitude);
	}

	bool operator>=(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	bool operator<(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return !(lhs >= rhs);
	}

	bool operator<=(ExtendedInteger lhs, const ExtendedInteger& rhs)
	{
		return !(lhs > rhs);
	}
}
