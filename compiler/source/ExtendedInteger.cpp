import std;

#include <utility/ExtendedInteger.h>

// Haze ExtendedInteger.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExtendedInteger::operator bool()
	{
		return static_cast<bool>(magnitude);
	}

	ExtendedInteger& ExtendedInteger::operator=(const ExtendedInteger& right)
	{
		magnitude = right.magnitude;
		sign = right.sign;
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator+=(const ExtendedInteger& right)
	{
		if (sign == right.sign)
		{
			magnitude += right.magnitude;
		}

		else
		{
			if (magnitude >= right.magnitude)
			{
				magnitude -= right.magnitude;
			}

			else
			{
				magnitude = right.magnitude - magnitude;
				sign = right.sign;
			}
		}

		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator-=(const ExtendedInteger& right)
	{
		if (sign != right.sign) 
		{
			magnitude += right.magnitude;
		}
		else 
		{
			if (magnitude >= right.magnitude) 
			{
				magnitude -= right.magnitude;
			}
			else 
			{
				magnitude = right.magnitude - magnitude;
				sign = !sign;
			}
		}

		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator*=(const ExtendedInteger& right)
	{
		magnitude *= right.magnitude;
		sign = (sign == right.sign);
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator/=(const ExtendedInteger& right)
	{
		magnitude /= right.magnitude;
		sign = (sign == right.sign);
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator%=(const ExtendedInteger& right)
	{
		magnitude %= right.magnitude;
		// C/C++ modulo by definition has the same sign as the left
		// for example, 5 % -3 = 2
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator&=(const ExtendedInteger& right)
	{
		magnitude &= right.magnitude;
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the left
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator^=(const ExtendedInteger& right)
	{
		magnitude ^= right.magnitude;
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the left
		return *this;
	}

	ExtendedInteger& ExtendedInteger::operator|=(const ExtendedInteger& right)
	{
		magnitude |= right.magnitude;
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the left
		return *this;
	}

	ExtendedInteger operator+(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left += right;
	}

	ExtendedInteger operator-(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left -= right;
	}

	ExtendedInteger operator*(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left *= right;
	}

	ExtendedInteger operator/(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left /= right;
	}

	ExtendedInteger operator%(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left %= right;
	}

	ExtendedInteger operator&(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left &= right;
	}

	ExtendedInteger operator^(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left ^= right;
	}

	ExtendedInteger operator|(ExtendedInteger left, const ExtendedInteger& right)
	{
		return left |= right;
	}

	ExtendedInteger operator~(const ExtendedInteger& right)
	{
		// sign with bitwise operations doesn't make sense as for arithmetic operators
		// so, ignore it and preserve the sign of the right
		return { ~right.magnitude, right.sign };
	}

	ExtendedInteger operator+(const ExtendedInteger& right)
	{
		return right;
	}

	ExtendedInteger operator-(const ExtendedInteger& right)
	{
		return { right.magnitude, !right.sign };
	}

	bool operator==(ExtendedInteger left, const ExtendedInteger& right)
	{
		return (left.magnitude == right.magnitude) && (left.sign == right.sign);
	}

	bool operator!=(ExtendedInteger left, const ExtendedInteger& right)
	{
		return !(left == right);
	}

	bool operator>(ExtendedInteger left, const ExtendedInteger& right)
	{
		if (left.sign != right.sign)
		{
			return left.sign;
		}

		return left.sign ? (left.magnitude > right.magnitude) : (left.magnitude < right.magnitude);
	}

	bool operator>=(ExtendedInteger left, const ExtendedInteger& right)
	{
		return (left > right) || (left == right);
	}

	bool operator<(ExtendedInteger left, const ExtendedInteger& right)
	{
		return !(left >= right);
	}

	bool operator<=(ExtendedInteger left, const ExtendedInteger& right)
	{
		return !(left > right);
	}
}
