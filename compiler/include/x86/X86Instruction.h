#ifndef HAZE_X86INSTRUCTION_H
#define HAZE_X86INSTRUCTION_H

#include <toolchain/models/Instruction.h>
#include <utility/Constants.h>
#include <x86/defs/X86Register.h>
#include <x86/defs/X86OperandKind.h>
#include <x86/defs/X86InstructionKind.h>

// Haze X86Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<template<typename> typename TraitT, typename... Ts>
		requires (TraitT<Ts>::value && ...)
	using ConstrainedVariant = std::variant<Ts...>;

	template<typename T>
	concept X86OperandConcept = requires(const T& t)
	{
		{ t.otype() } -> std::same_as<X86OperandKind>;
	};

	template<typename T>
	struct X86OperandTrait
	{
		static constexpr bool value = X86OperandConcept<T>;
	};

	namespace x86
	{
		class ImmediateOperand
		{
		public:
			ExtendedInteger immediate;

		public:
			ImmediateOperand(const ExtendedInteger& immediate)
				: immediate{ immediate }
			{
			}

		public:
			X86OperandKind otype() const;
		};
#define imm(value) ImmediateOperand{ value }

		class IndirectOperand
		{
		public:
			Address address;

		public:
			IndirectOperand(Address address)
				: address{ address }
			{
			}

		public:
			X86OperandKind otype() const;
		};
#define indirect(address) IndirectOperand{ address }

		class RegisterOperand
		{ 
		public:
			X86Register $register;

		public:
			RegisterOperand(X86Register $register)
				: $register{ $register }
			{
			}

		public:
			X86OperandKind otype() const;
		};
#define reg(reg) RegisterOperand{ reg }

		class RegisterIndirectOperand : public RegisterOperand
		{
		public:
			using RegisterOperand::RegisterOperand;

		public:
			X86OperandKind otype() const;
		};
#define reg_indirect($register) RegisterIndirectOperand{ $register }

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
			X86OperandKind otype() const;
		};
#define reg_disp($register, disp) RegisterDisplacedOperand{ $register, disp }
	}

	using X86Operand = ConstrainedVariant<X86OperandTrait,
		x86::ImmediateOperand,
		x86::IndirectOperand,
		x86::RegisterOperand,
		x86::RegisterIndirectOperand,
		x86::RegisterDisplacedOperand
	>;

	X86OperandKind operand_type(const X86Operand& operand)
	{
		return std::visit([]<X86OperandConcept T>(const T& variant)
		{
			return variant.otype();
		}, operand);
	}



	template<typename T>
	concept X86InstructionConcept = requires(const T & t)
	{
		{ t.itype() } -> std::same_as<X86InstructionKind>;
		{ t.emit() } -> std::same_as<ByteRange>;
	};

	template<typename T>
	struct X86InstructionTrait
	{
		static constexpr bool value = X86InstructionConcept<T>;
	};

	namespace x86
	{
		class PushInstruction
		{
		private:
			X86Operand operand;

		public:
			PushInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define push(op) PushInstruction{ op }

		class PopInstruction
		{
		private:
			X86Operand operand;

		public:
			PopInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define pop(op) PopInstruction{ op }

		class MovInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			MovInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define mov(dst, src) MovInstruction{ dst, src }

		class MovzxInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			MovzxInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source  }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define movzx(dst, src) MovzxInstruction{ dst, src }

		class AddInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			AddInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source  }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define add(dst, src) AddInstruction{ dst, src }

		class SubInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			SubInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source  }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define sub(dst, src) SubInstruction{ dst, src }

		class AndInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			AndInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source  }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define and(dst, src) AndInstruction{ dst, src }

		class OrInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			OrInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define or(dst, src) OrInstruction{ dst, src }

		class XorInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			XorInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define xor(dst, src) XorInstruction{ dst, src }

		class IncInstruction
		{
		private:
			X86Operand operand;

		public:
			IncInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define inc(op) IncInstruction{ op }

		class DecInstruction
		{
		private:
			X86Operand operand;

		public:
			DecInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define dec(op) DecInstruction{ op }

		class SalInstruction
		{
		private:
			X86Operand operand;
			Offset immediate;

		public:
			SalInstruction(const X86Operand& operand, Offset immediate = -1)
				: operand{ operand }, immediate{ immediate }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define sal(op) SalInstruction{ op }

		class SarInstruction
		{
		private:
			X86Operand operand;
			Offset immediate;

		public:
			SarInstruction(const X86Operand& operand, Offset immediate = -1)
				: operand{ operand }, immediate{ immediate }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define sar(op) SarInstruction{ op }

		class TestInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			TestInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source  }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define test(dst, src) TestInstruction{ dst, src }

		class CmpInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			CmpInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source  }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define cmp(dst, src) CmpInstruction{ dst, src }

		class CallInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			CallInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define call(label, disp) CallInstruction{ label, disp }

		class ApicallInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			ApicallInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define apicall(label, disp) ApicallInstruction{ label, disp }

		class JmpInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JmpInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jmp(label, disp) JmpInstruction{ label, disp }

		class JeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define je(label, disp) JeInstruction{ label, disp }

		class JneInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JneInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jne(label, disp) JneInstruction{ label, disp }

		class JlInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JlInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jl(label, disp) JlInstruction{ label, disp }

		class JleInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JleInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jle(label, disp) JleInstruction{ label, disp }

		class JgInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JgInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jg(label, disp) JgInstruction{ label, disp }

		class JgeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JgeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jge(label, disp) JgeInstruction{ label, disp }

		class JaInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JaInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define ja(label, disp) JaInstruction{ label, disp }

		class JaeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JaeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jae(label, disp) JaeInstruction{ label, disp }

		class JbInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JbInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jb(label, disp) JbInstruction{ label, disp }

		class JbeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			JbeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define jbe(label, disp) JbeInstruction{ label, disp }

		class SeteInstruction
		{
		private:
			X86Operand operand;

		public:
			SeteInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define sete(op) SeteInstruction{ op }

		class SetneInstruction
		{
		private:
			X86Operand operand;

		public:
			SetneInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setne(op) SetneInstruction{ op }

		class SetlInstruction
		{
		private:
			X86Operand operand;

		public:
			SetlInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setl(op) SetlInstruction{ op }

		class SetleInstruction
		{
		private:
			X86Operand operand;

		public:
			SetleInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setle(op) SetleInstruction{ op }

		class SetgInstruction
		{
		private:
			X86Operand operand;

		public:
			SetgInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setg(op) SetgInstruction{ op }

		class SetgeInstruction
		{
		private:
			X86Operand operand;

		public:
			SetgeInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setge(op) SetgeInstruction{ op }

		class SetaInstruction
		{
		private:
			X86Operand operand;

		public:
			SetaInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define seta(op) SetaInstruction{ op }

		class SetaeInstruction
		{
		private:
			X86Operand operand;

		public:
			SetaeInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setae(op) SetaeInstruction{ op }

		class SetbInstruction
		{
		private:
			X86Operand operand;

		public:
			SetbInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setb(op) SetbInstruction{ op }

		class SetbeInstruction
		{
		private:
			X86Operand operand;

		public:
			SetbeInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define setbe(op) SetbeInstruction{ op }

		class RetInstruction
		{
		private:
			Offset immediate;

		public:
			RetInstruction(Offset immediate = -1)
				: immediate{ immediate }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define ret() RetInstruction{}

		class LeaveInstruction
		{
		// default constructor
		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define leave() LeaveInstruction{}
	}

	using X86Instruction = ConstrainedVariant<X86InstructionTrait,
		x86::PushInstruction,
		x86::PopInstruction,
		x86::MovInstruction,
		x86::MovzxInstruction,
		x86::AddInstruction,
		x86::SubInstruction,
		x86::AndInstruction,
		x86::OrInstruction,
		x86::XorInstruction,
		x86::IncInstruction,
		x86::DecInstruction,
		x86::SalInstruction,
		x86::SarInstruction,
		x86::TestInstruction,
		x86::CmpInstruction,
		x86::CallInstruction,
		x86::ApicallInstruction,
		x86::JmpInstruction,
		x86::JeInstruction,
		x86::JneInstruction,
		x86::JlInstruction,
		x86::JleInstruction,
		x86::JgInstruction,
		x86::JgeInstruction,
		x86::JaInstruction,
		x86::JaeInstruction,
		x86::JbInstruction,
		x86::JbeInstruction,
		x86::SeteInstruction,
		x86::SetneInstruction,
		x86::SetlInstruction,
		x86::SetleInstruction,
		x86::SetgInstruction,
		x86::SetgeInstruction,
		x86::SetaInstruction,
		x86::SetaeInstruction,
		x86::SetbInstruction,
		x86::SetbeInstruction,
		x86::RetInstruction,
		x86::LeaveInstruction
	> ;

	X86InstructionKind instruction_type(const X86Instruction& instruction)
	{
		return std::visit([]<X86InstructionConcept T>(const T& variant)
		{
			return variant.itype();
		}, instruction);
	}

	ByteRange emit(const X86Instruction& instruction)
	{
		return std::visit([]<X86InstructionConcept T>(const T& variant)
		{
			return variant.emit();
		}, instruction);
	}
}

#endif
