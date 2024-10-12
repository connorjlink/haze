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
		std::string identifier;

	public:
		Variable(const std::string& identifier)
			: identifier{ identifier }
		{
		}

	public:
		virtual VariableType vtype() const = 0;
		virtual std::string format() const = 0;
	};

	class UnsignedByteVariable : public Variable
	{
	public:
		std::uint8_t value;

	public:
		UnsignedByteVariable(const std::string& identifier, std::uint8_t value)
			: Variable{ identifier }, value{ value }
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
		SignedByteVariable(const std::string& identifier, std::int8_t value)
			: Variable{ identifier }, value{ value }
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
		UnsignedWordVariable(const std::string& identifier, std::uint16_t value)
			: Variable{ identifier }, value{ value }
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
		SignedWordVariable(const std::string& identifier, std::int16_t value)
			: Variable{ identifier }, value{ value }
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
		UnsignedDoubleWordVariable(const std::string& identifier, std::uint32_t value)
			: Variable{ identifier }, value{ value }
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
		SignedDoubleWordVariable(const std::string& identifier, std::int32_t value)
			: Variable{ identifier }, value{ value }
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
		UnsignedQuadWordVariable(const std::string& identifier, std::uint64_t value)
			: Variable{ identifier }, value{ value }
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
		SignedQuadWordVariable(const std::string& identifier, std::int64_t value)
			: Variable{ identifier }, value{ value }
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
		StringVariable(const std::string& identifier, const std::string& value)
			: Variable{ identifier }, value{ value }
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
		StructVariable(const std::string& identifier, const std::string& tag)
			: Variable{ identifier }, tag{ tag }
		{
		}

	public:
		virtual VariableType vtype() const final override;
		virtual std::string format() const final override;
	};
}

#endif
