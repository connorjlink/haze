#ifndef HAZE_VARIABLE_H
#define HAZE_VARIABLE_H

#include "VariableType.h"

// Haze Variable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Variable
	{
	public:
		virtual VariableType vtype() const = 0;
		virtual std::string format() const = 0;
	};

	class UnsignedByteVariable : public Variable
	{
	public:
		std::uint8_t value;

	public:
		UnsignedByteVariable(std::uint8_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class SignedByteVariable : public Variable
	{
	public:
		std::int8_t value;

	public:
		SignedByteVariable(std::int8_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class UnsignedWordVariable : public Variable
	{
	public:
		std::uint16_t value;

	public:
		UnsignedWordVariable(std::uint16_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class SignedWordVariable : public Variable
	{
	public:
		std::int16_t value;

	public:
		SignedWordVariable(std::int16_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class UnsignedDoubleWordVariable : public Variable
	{
	public:
		std::uint32_t value;

	public:
		UnsignedDoubleWordVariable(std::uint32_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class SignedDoubleWordVariable : public Variable
	{
	public:
		std::int32_t value;

	public:
		SignedDoubleWordVariable(std::int32_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class UnsignedQuadWordVariable : public Variable
	{
	public:
		std::uint64_t value;

	public:
		UnsignedQuadWordVariable(std::uint64_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class SignedQuadWordVariable : public Variable
	{
	public:
		std::int64_t value;

	public:
		SignedQuadWordVariable(std::int64_t value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class StringVariable : public Variable
	{
	public:
		std::string value;

	public:
		StringVariable(const std::string& value)
			: value{ value }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};

	class StructVariable : public Variable
	{
	public:
		std::string tag;

	public:
		StructVariable(const std::string& tag)
			: tag{ tag }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};
}

#endif
