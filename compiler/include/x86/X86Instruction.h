#ifndef HAZE_X86INSTRUCTION_H
#define HAZE_X86INSTRUCTION_H

#include <utility/Constants.h>
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
			ImmediateOperand(const ExtendedInteger& immediate)
				: immediate{ immediate }
			{
			}

		public:
			X86OperandKind otype() const;
		};
#define imm(value) ImmediateOperand{ value }

		struct IndirectOperand
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

		struct RegisterOperand
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
#define $register($register) RegisterOperand{ $register }

		struct RegisterIndirectOperand : public RegisterOperand
		{
		public:
			using RegisterOperand::RegisterOperand;

		public:
			X86OperandKind otype() const;
		};
#define reg_indirect($register) RegisterIndirectOperand{ $register }

		struct RegisterDisplacedOperand : public RegisterOperand
		{
		public:
			Offset displacement;

		public:
			RegisterDisplacedOperand(X86Register $register, Offset displacement)
				: RegisterOperand{ $register }, displacement{ displacement }
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
			PushInstruction(const X86Operand& operand)
				: operand{ operand }
			{
			}

		public:
			X86InstructionKind itype() const;
			ByteRange emit() const;
		};
#define push(op) PushInstruction{ op }

		struct PopInstruction
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

		struct MovInstruction
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

		struct MovzxInstruction
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

		struct AddInstruction
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

		struct SubInstruction
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

		struct AndInstruction
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

		struct OrInstruction
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

		struct XorInstruction
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

		struct IncInstruction
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

		struct DecInstruction
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

		struct SalInstruction
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

		struct SarInstruction
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

		struct TestInstruction
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

		struct CmpInstruction
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

		struct CallInstruction
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

		struct ApicallInstruction
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

		struct JmpInstruction
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

		struct JeInstruction
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

		struct JneInstruction
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

		struct JlInstruction
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

		struct JleInstruction
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

		struct JgInstruction
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

		struct JgeInstruction
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

		struct JaInstruction
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

		struct JaeInstruction
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

		struct JbInstruction
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

		struct JbeInstruction
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

		struct SeteInstruction
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

		struct SetneInstruction
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

		struct SetlInstruction
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

		struct SetleInstruction
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

		struct SetgInstruction
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

		struct SetgeInstruction
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

		struct SetaInstruction
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

		struct SetaeInstruction
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

		struct SetbInstruction
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

		struct SetbeInstruction
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

		struct RetInstruction
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

		struct LeaveInstruction
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
