#ifndef HAZE_X86INSTRUCTION_H
#define HAZE_X86INSTRUCTION_H

#include <utility/Constants.h>
#include <utility/ExtendedInteger.h>
#include <utility/Variant.h>
#include <x86/defs/X86Register.h>
#include <x86/defs/X86OperandKind.h>
#include <x86/defs/X86InstructionKind.h>

// Haze X86Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	concept IsX86Operand = requires(const T& t)
	{
		{ t.otype() } -> std::same_as<X86OperandKind>;
	};

	template<typename T>
	struct X86OperandTrait
	{
		static constexpr bool value = IsX86Operand<T>;
	};

	namespace x86
	{
		struct ImmediateOperand
		{
		public:
			ExtendedInteger immediate;

		public:
			X86OperandKind otype() const;

		public:
			ImmediateOperand(const ExtendedInteger& immediate)
				: immediate{ immediate }
			{
			}
		};
#define immediate(value) ImmediateOperand{ value }

		struct IndirectOperand
		{
		public:
			Address address;

		public:
			X86OperandKind otype() const;

		public:
			IndirectOperand(Address address)
				: address{ address }
			{
			}
		};
#define indirect(address) IndirectOperand{ address }

		struct RegisterOperand
		{ 
		public:
			X86Register $register;

		public:
			X86OperandKind otype() const;

		public:
			RegisterOperand(X86Register $register)
				: $register{ $register }
			{
			}
		};
#define $register(register) RegisterOperand{ register }

		struct RegisterIndirectOperand : public RegisterOperand
		{
		public:
			X86OperandKind otype() const;

		public:
			using RegisterOperand::RegisterOperand;
		};
#define register_indirect(register) RegisterIndirectOperand{ register }

		struct RegisterDisplacedOperand : public RegisterOperand
		{
		public:
			Offset displacement;

		public:
			X86OperandKind otype() const;

		public:
			RegisterDisplacedOperand(X86Register $register, Offset displacement)
				: RegisterOperand{ $register }, displacement{ displacement }
			{
			}
		};
#define reg_disp($register, displacement) RegisterDisplacedOperand{ $register, displacement }
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
		return std::visit([]<IsX86Operand T>(const T& variant)
		{
			return variant.otype();
		}, operand);
	}



	template<typename T>
	concept IsX86Instruction = requires(const T & t)
	{
		{ t.itype() } -> std::same_as<X86InstructionKind>;
		{ t.emit() } -> std::same_as<ByteRange>;
	};

	template<typename T>
	struct X86InstructionTrait
	{
		static constexpr bool value = IsX86Instruction<T>;
	};

	namespace x86
	{
		struct PushInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			PushInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define push(operand) PushInstruction{ operand }

		struct PopInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			PopInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define pop(operand) PopInstruction{ operand }

		struct MovInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			MovInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define mov(destination, source) MovInstruction{ destination, source }

		struct MovzxInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			MovzxInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define movzx(destination, source) MovzxInstruction{ destination, source }

		struct AddInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			AddInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define add(destination, source) AddInstruction{ destination, source }

		struct SubInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SubInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define sub(destination, source) SubInstruction{ destination, source }

		struct AndInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			AndInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define and(destination, source) AndInstruction{ destination, source }

		struct OrInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			OrInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define or(destination, source) OrInstruction{ destination, source }

		struct XorInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			XorInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define xor(destination, source) XorInstruction{ destination, source }

		struct IncInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			IncInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define inc(op) IncInstruction{ op }

		struct DecInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			DecInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define dec(op) DecInstruction{ op }

		struct SalInstruction
		{
		private:
			X86Operand operand;
			Offset immediate;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SalInstruction(const X86Operand& operand, Offset immediate = -1)
				: operand{ operand }, immediate{ immediate }
			{
			}
		};
#define sal(op) SalInstruction{ op }

		struct SarInstruction
		{
		private:
			X86Operand operand;
			Offset immediate;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SarInstruction(const X86Operand& operand, Offset immediate = -1)
				: operand{ operand }, immediate{ immediate }
			{
			}
		};
#define sar(op) SarInstruction{ op }

		struct TestInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			TestInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define test(destination, source) TestInstruction{ destination, source }

		struct CmpInstruction
		{
		private:
			X86Operand destination;
			X86Operand source;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			CmpInstruction(const X86Operand& destination, const X86Operand& source)
				: destination{ destination }, source{ source }
			{
			}
		};
#define cmp(destination, source) CmpInstruction{ destination, source }

		struct CallInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			CallInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define call(label, displacement) CallInstruction{ label, displacement }

		struct ApicallInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			ApicallInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define apicall(label, displacement) ApicallInstruction{ label, displacement }

		struct JmpInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JmpInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jmp(label, displacement) JmpInstruction{ label, displacement }

		struct JeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define je(label, displacement) JeInstruction{ label, displacement }

		struct JneInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JneInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jne(label, displacement) JneInstruction{ label, displacement }

		struct JlInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JlInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jl(label, displacement) JlInstruction{ label, displacement }

		struct JleInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JleInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jle(label, displacement) JleInstruction{ label, displacement }

		struct JgInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JgInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jg(label, displacement) JgInstruction{ label, displacement }

		struct JgeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JgeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jge(label, displacement) JgeInstruction{ label, displacement }

		struct JaInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JaInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define ja(label, displacement) JaInstruction{ label, displacement }

		struct JaeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JaeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jae(label, displacement) JaeInstruction{ label, displacement }

		struct JbInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JbInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jb(label, displacement) JbInstruction{ label, displacement }

		struct JbeInstruction
		{
		private:
			const std::string& label;
			Offset displacement;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			JbeInstruction(const std::string& label, Offset displacement)
				: label{ label }, displacement{ displacement }
			{
			}
		};
#define jbe(label, displacement) JbeInstruction{ label, displacement }

		struct SeteInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SeteInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define sete(operand) SeteInstruction{ operand }

		struct SetneInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetneInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setne(operand) SetneInstruction{ operand }

		struct SetlInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetlInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setl(operand) SetlInstruction{ operand }

		struct SetleInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetleInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setle(operand) SetleInstruction{ operand }

		struct SetgInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetgInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setg(operand) SetgInstruction{ operand }

		struct SetgeInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetgeInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setge(operand) SetgeInstruction{ operand }

		struct SetaInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetaInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define seta(operand) SetaInstruction{ operand }

		struct SetaeInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetaeInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setae(operand) SetaeInstruction{ operand }

		struct SetbInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetbInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setb(operand) SetbInstruction{ operand }

		struct SetbeInstruction
		{
		private:
			X86Operand operand;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			SetbeInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}
		};
#define setbe(operand) SetbeInstruction{ operand }

		struct RetInstruction
		{
		private:
			Offset immediate;

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			RetInstruction(Offset immediate = -1)
				: immediate{ immediate }
			{
			}
		};
#define ret() RetInstruction{}

		struct LeaveInstruction
		{
		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;

		public:
			// default constructor
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
		return std::visit([]<IsX86Instruction T>(const T& variant)
		{
			return variant.itype();
		}, instruction);
	}

	ByteRange emit(const X86Instruction& instruction)
	{
		return std::visit([]<IsX86Instruction T>(const T& variant)
		{
			return variant.emit();
		}, instruction);
	}
}

#endif
