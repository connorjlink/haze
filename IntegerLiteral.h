#ifndef HAZE_INTEGERLITERAL_H
#define HAZE_INTEGERLITERAL_H

#include "IntegerLiteralType.h"

// Haze IntegerLiteral.h
// (c) Connor J. Link. All Rights Reserved.

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

	using _ir_t = std::int32_t;

	_ir_t integer_literal_raw(IntegerLiteral* value)
	{
#pragma message("TODO: figure out how to return these values in an agnostic way that is compatibrl with all types")
		using enum IntegerLiteralType;
		switch (value->itype())
		{
			case UBYTE: return static_cast<_ir_t>(AS_UNSIGNED_BYTE_INTEGER_LITERAL(value)->value);
			case SBYTE: return static_cast<_ir_t>(AS_SIGNED_BYTE_INTEGER_LITERAL(value)->value);

			case UWORD: return static_cast<_ir_t>(AS_UNSIGNED_WORD_INTEGER_LITERAL(value)->value);
			case SWORD: return static_cast<_ir_t>(AS_SIGNED_WORD_INTEGER_LITERAL(value)->value);

			case UDWORD: return static_cast<_ir_t>(AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(value)->value);
			case SDWORD: return static_cast<_ir_t>(AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(value)->value);

			case UQWORD: return static_cast<_ir_t>(AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(value)->value);
			case SQWORD: return static_cast<_ir_t>(AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(value)->value);
		}
	}



	template<typename T>
	bool integer_literal_compare(IntegerLiteral* value, _ir_t comparison)
	{
		return T{}(_ir_t{ integer_literal_raw(value) }, comparison);
	}

	bool integer_literal_equals(IntegerLiteral* value, _ir_t comparison)
	{
		return integer_literal_compare<std::equal_to<_ir_t>>(value, comparison);
	}
}

#endif
