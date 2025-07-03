#ifndef HAZE_INTEGERLITERAL_H
#define HAZE_INTEGERLITERAL_H

#include "IntegerLiteralType.h"
#include "CommonErrors.h"
#include "PlatformVariables.h"
#include "Variable.h"
#include "Token.h"
#include "ExtendedInteger.h"

// Haze IntegerLiteral.h
// (c) Connor J. Link. All Rights Reserved.

#define UBYTE(x) UnsignedByteIntegerLiteral{ x }
#define SBYTE(x) SignedByteIntegerLiteral{ x }
#define UWORD(x) UnsignedWordIntegerLiteral{ x }
#define SWORD(x) SignedWordIntegerLiteral{ x }
#define UDWORD(x) UnsignedDoubleWordIntegerLiteral{ x }
#define SDWORD(x) SignedDoubleWordIntegerLiteral{ x }
#define UQWORD(x) UnsignedQuadWordIntegerLiteral{ x }
#define SQWORD(x) SignedQuadWordIntegerLiteral{ x }

#define UNSIGNED_BYTE_CAST(x) *AS_UNSIGNED_BYTE_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define SIGNED_BYTE_CAST(x) *AS_SIGNED_BYTE_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define UNSIGNED_WORD_CAST(x) *AS_UNSIGNED_WORD_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define SIGNED_WORD_CAST(x) *AS_SIGNED_WORD_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define UNSIGNED_DOUBLE_WORD_CAST(x) *AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define SIGNED_DOUBLE_WORD_CAST(x) *AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define UNSIGNED_QUAD_WORD_CAST(x) *AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)
#define SIGNED_QUAD_WORD_CAST(x) *AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(AS_INTEGER_LITERAL_EXPRESSION(x)->value)

#define VALUE_OF(x) AS_INTEGER_LITERAL_EXPRESSION(x)->value
#define MESSAGE_OF(x) AS_STRING_EXPRESSION(x)->message

#define TEST_VALUE_EQUALS(x, y) integer_literal_raw(VALUE_OF(x)->equals(VALUE_OF(x)->from_value(y)))

namespace hz
{
	class IntegerLiteral
	{
	public:
		//Leave compiler generated default constructor in place

	public:
		virtual IntegerLiteral* from_value(std::int32_t) const = 0;

	public:
		virtual IntegerLiteralType itype() const = 0;
		virtual IntegerLiteral* plus(IntegerLiteral*) = 0;
		virtual IntegerLiteral* minus(IntegerLiteral*) = 0;
		virtual IntegerLiteral* times(IntegerLiteral*) = 0;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) = 0;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) = 0;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) = 0;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) = 0;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) = 0;
		virtual IntegerLiteral* greater(IntegerLiteral*) = 0;
		virtual IntegerLiteral* less(IntegerLiteral*) = 0;
		virtual IntegerLiteral* equals(IntegerLiteral*) = 0;
		virtual IntegerLiteral* notequals(IntegerLiteral*) = 0;
		virtual IntegerLiteral* assign(IntegerLiteral*) = 0;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new UnsignedByteIntegerLiteral{ static_cast<std::uint8_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new SignedByteIntegerLiteral{ static_cast<std::int8_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new UnsignedWordIntegerLiteral{ static_cast<std::uint16_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new SignedWordIntegerLiteral{ static_cast<std::int16_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new UnsignedDoubleWordIntegerLiteral{ static_cast<std::uint32_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new SignedDoubleWordIntegerLiteral{ static_cast<std::int32_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new UnsignedQuadWordIntegerLiteral{ static_cast<std::uint64_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
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
		virtual IntegerLiteral* from_value(std::int32_t value) const final override
		{
			return new SignedQuadWordIntegerLiteral{ static_cast<std::int64_t>(value) };
		}

	public:
		virtual IntegerLiteralType itype() const final override;
		virtual IntegerLiteral* plus(IntegerLiteral*) final override;
		virtual IntegerLiteral* minus(IntegerLiteral*) final override;
		virtual IntegerLiteral* times(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisexor(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiseand(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwisershift(IntegerLiteral*) final override;
		virtual IntegerLiteral* bitwiselshift(IntegerLiteral*) final override;
		virtual IntegerLiteral* greater(IntegerLiteral*) final override;
		virtual IntegerLiteral* less(IntegerLiteral*) final override;
		virtual IntegerLiteral* assign(IntegerLiteral*) final override;
		virtual IntegerLiteral* equals(IntegerLiteral*) final override;
		virtual IntegerLiteral* notequals(IntegerLiteral*) final override;
	};

	ExtendedInteger integer_literal_raw(IntegerLiteral*);


	template<typename T>
	bool integer_literal_compare(IntegerLiteral* value, ExtendedInteger comparison)
	{
		return T{}(ExtendedInteger{ integer_literal_raw(value) }, comparison);
	}

	bool integer_literal_equals(IntegerLiteral*, ExtendedInteger);

	template<typename T>
		requires (std::is_same_v<T, std::uint8_t> or
				  std::is_same_v<T, std::int8_t> or
				  std::is_same_v<T, std::uint16_t> or
				  std::is_same_v<T, std::int16_t> or
				  std::is_same_v<T, std::uint32_t> or
				  std::is_same_v<T, std::int32_t> or
			      std::is_same_v<T, std::uint64_t> or
				  std::is_same_v<T, std::int64_t>)
	IntegerLiteral* make_integer_literal(T value)
	{
		static_assert(false);
	}

	template<>
	IntegerLiteral* make_integer_literal<std::uint8_t>(std::uint8_t);

	template<>
	IntegerLiteral* make_integer_literal<std::int8_t>(std::int8_t);
	
	template<>
	IntegerLiteral* make_integer_literal<std::uint16_t>(std::uint16_t);
	
	template<>
	IntegerLiteral* make_integer_literal<std::int16_t>(std::int16_t);
	
	template<>
	IntegerLiteral* make_integer_literal<std::uint32_t>(std::uint32_t);

	template<>
	IntegerLiteral* make_integer_literal<std::int32_t>(std::int32_t);

	template<>
	IntegerLiteral* make_integer_literal<std::uint64_t>(std::uint64_t);

	template<>
	IntegerLiteral* make_integer_literal<std::int64_t>(std::int64_t);

	// all possible explicit specializations are defined in the cpp file

	Variable* integer_literal_to_variable(IntegerLiteral*);
}

#endif
