#ifndef HAZE_X86OPERAND_H
#define HAZE_X86OPERAND_H

#include <x86/X86OperandType.h>
#include <x86/X86Register.h>
#include <utility/ExtendedInteger.h>

// Haze X86Operand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Operand
	{
	public:
		virtual X86OperandType otype() const = 0;
	};

	namespace x86
	{
		class ImmediateOperand : public X86Operand
		{
		public:
			ExtendedInteger immediate;

		public:
			ImmediateOperand(const ExtendedInteger& immediate)
				: immediate{ immediate }
			{
			}

		public:
			virtual X86OperandType otype() const final override;
		};

		class IndirectOperand : public X86Operand
		{
		public:
			Address address;

		public:
			IndirectOperand(Address address)
				: address{ address }
			{
			}

		public:
			virtual X86OperandType otype() const final override;
		};

		class RegisterOperand : public X86Operand
		{
		public:
			X86Register register;

		public:
			RegisterOperand(X86Register reg)
				: register{ reg }
			{
			}

		public:
			virtual X86OperandType otype() const override;
		};

		class RegisterIndirectOperand : public RegisterOperand
		{
		public:
			using RegisterOperand::RegisterOperand;

		public:
			virtual X86OperandType otype() const final override;
		};

		class RegisterDisplacedOperand : public RegisterOperand
		{
		public:
			Offset displacement;

		public:
			RegisterDisplacedOperand(X86Register reg, Offset displacement)
				: RegisterOperand{ reg }, displacement{ displacement }
			{
			}
			

		public:
			virtual X86OperandType otype() const final override;
		};
	}
}

#endif
