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
			ExtendedInteger _immediate;

		public:
			ImmediateOperand(const ExtendedInteger& immediate)
				: _immediate{ immediate }
			{
			}

		public:
			virtual X86OperandType otype() const final override;
		};

		class IndirectOperand : public X86Operand
		{
		public:
			native_uint _address;

		public:
			IndirectOperand(native_uint address)
				: _address{ address }
			{
			}

		public:
			virtual X86OperandType otype() const final override;
		};

		class RegisterOperand : public X86Operand
		{
		public:
			X86Register _register;

		public:
			RegisterOperand(X86Register reg)
				: _register{ reg }
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
			native_int _displacement;

		public:
			RegisterDisplacedOperand(X86Register reg, native_int displacement)
				: RegisterOperand{ reg }, _displacement{ displacement }
			{
			}
			

		public:
			virtual X86OperandType otype() const final override;
		};
	}
}

#endif
