#ifndef HAZE_INTEGERLITERAL_H
#define HAZE_INTEGERLITERAL_H

#include "IntegerLiteralType.h"

// Haze IntegerLiteral.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	union IntegerLiteralStorage
	{
		std::uint8_t ubyte;
		std::int8_t sbyte;

		std::uint16_t uword;
		std::int16_t sword;

		std::uint32_t udword;
		std::int32_t sdword;

		std::uint64_t uqword;
		std::int64_t sqword;
	};

	class IntegerLiteral
	{
	public:
		IntegerLiteralStorage storage;
		IntegerLiteralType type;

	public:
		IntegerLiteral(IntegerLiteralStorage storage, IntegerLiteralType type)
			: storage{ storage }, type{ type }
		{
		}

		IntegerLiteral() = default;

	public:
		virtual IntegerLiteralType itype() const = 0;
		virtual IntegerLiteral& operator+(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator-(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator*(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator&(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator|(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator^(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator>(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator<(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator==(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) = 0;
		virtual IntegerLiteral& operator=(IntegerLiteral&) = 0;
	};

	class UnsignedByteIntegerLiteral : public IntegerLiteral
	{
	public:
		std::uint8_t value;
		
	public:
		UnsignedByteIntegerLiteral(std::uint8_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
	};

	class SignedByteIntegerLiteral : public IntegerLiteral
	{
	public:
		std::int8_t value;

	public:
		SignedByteIntegerLiteral(std::int8_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};

	class UnsignedWordIntegerLiteral : public IntegerLiteral
	{
	public:
		std::uint16_t value;

	public:
		UnsignedWordIntegerLiteral(std::uint16_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};

	class SignedWordIntegerLiteral : public IntegerLiteral
	{
	public:
		std::int16_t value;

	public:
		SignedWordIntegerLiteral(std::int16_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};

	class UnsignedDoubleWordIntegerLiteral : public IntegerLiteral
	{
	public:
		std::uint32_t value;

	public:
		UnsignedDoubleWordIntegerLiteral(std::uint32_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};

	class SignedDoubleWordIntegerLiteral : public IntegerLiteral
	{
	public:
		std::int32_t value;

	public:
		SignedDoubleWordIntegerLiteral(std::int32_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};

	class UnsignedQuadWordIntegerLiteral : public IntegerLiteral
	{
	public:
		std::uint64_t value;

	public:
		UnsignedQuadWordIntegerLiteral(std::uint64_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};
	
	class SignedQuadWordIntegerLiteral : public IntegerLiteral
	{
	public:
		std::int64_t value;

	public:
		SignedQuadWordIntegerLiteral(std::int64_t value)
			: value{ value }
		{
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral& operator+(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator-(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator*(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator&(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator|(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator^(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator>(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator<(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator=(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator==(IntegerLiteral&) final override;
		virtual IntegerLiteral& operator!=(IntegerLiteral&) final override;
	};
}

#endif
