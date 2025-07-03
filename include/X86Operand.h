#ifndef HAZE_X86OPERAND_H
#define HAZE_X86OPERAND_H

#include "X86OperandType.h"
#include "X86Register.h"

// Haze X86Operand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Operand
	{
	public:
		virtual X86OperandType otype() const = 0;
	};

	class X86ImmediateOperand : public X86Operand
	{
	public:
		std::int32_t _immediate;

	public:
		using X86Operand::X86Operand;

	public:
		virtual X86OperandType otype() const final override;
	};

	class X86IndirectOperand : public X86Operand
	{
	public:
		std::uint32_t _address;

	public:
		using X86Operand::X86Operand;

	public:
		virtual X86OperandType otype() const final override;
	};

	class X86RegisterOperand : public X86Operand
	{
	public:
		X86Register _register;

	public:
		using X86Operand::X86Operand;

	public:
		virtual X86OperandType otype() const override;
	};

	class X86RegisterIndirectOperand : public X86RegisterOperand
	{
	public:
		using X86RegisterOperand::X86RegisterOperand;

	public:
		virtual X86OperandType otype() const final override;
	};

	class X86RegisterDisplacedOperand : public X86RegisterOperand
	{
	public:
		std::int32_t _displacement;

	public:
		using X86RegisterOperand::X86RegisterOperand;

	public:
		virtual X86OperandType otype() const final override;
	};
}

#endif
