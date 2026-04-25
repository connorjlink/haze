#ifndef HAZE_X86INSTRUCTION_H
#define HAZE_X86INSTRUCTION_H

#include <toolchain/models/Instruction.h>
#include <utility/Constants.h>

// Haze X86Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86OperandType
	{
		IMMEDIATE,
		INDIRECT,
		REGISTER,
		REGISTER_INDIRECT,
		REGISTER_DISPLACED,
	};

	static const std::unordered_map<X86OperandType, std::string_view> operand_type_map
	{
		{ X86OperandType::IMMEDIATE, "immediate" },
		{ X86OperandType::INDIRECT, "indirect" },
		{ X86OperandType::REGISTER, "register" },
		{ X86OperandType::REGISTER_INDIRECT, "register-indirect" },
		{ X86OperandType::REGISTER_DISPLACED, "register-displaced" },
	};


	enum class X86InstructionType
	{
		PUSH, POP,
		MOV, MOVZX,
		ADD, SUB,
		OR, AND, XOR,
		INC, DEC,
		SAL, SAR,
		TEST, CMP,
		// jcc
		CALL, APICALL, JMP, JE, JNE, JL, JLE, JG, JGE, JA, JAE, JB, JBE,
		// setcc
		SETE, SETNE, SETL, SETLE, SETG, SETGE, SETA, SETAE, SETB, SETBE,
		NOP,
		RET,
		LEAVE,
	};

	static const std::unordered_map<X86InstructionType, std::string_view> instruction_type_map
	{
		{ X86InstructionType::PUSH, "push" },
		{ X86InstructionType::POP, "pop" },
		{ X86InstructionType::MOV, "mov" },
		{ X86InstructionType::MOVZX, "movzx" },
		{ X86InstructionType::ADD, "add" },
		{ X86InstructionType::SUB, "sub" },
		{ X86InstructionType::OR, "or" },
		{ X86InstructionType::AND, "and" },
		{ X86InstructionType::XOR, "xor" },
		{ X86InstructionType::INC, "inc" },
		{ X86InstructionType::DEC, "dec" },
		{ X86InstructionType::SAL, "sal" },
		{ X86InstructionType::SAR, "sar" },
		{ X86InstructionType::TEST, "test" },
		{ X86InstructionType::CMP, "cmp" },
		{ X86InstructionType::CALL, "call" },
		{ X86InstructionType::APICALL, "apicall" },
		{ X86InstructionType::JMP, "jmp" },
		{ X86InstructionType::JE, "je" },
		{ X86InstructionType::JNE, "jne" },
		{ X86InstructionType::JL, "jl" },
		{ X86InstructionType::JLE, "jle" },
		{ X86InstructionType::JG, "jg" },
		{ X86InstructionType::JGE, "jge" },
		{ X86InstructionType::JA, "ja" },
		{ X86InstructionType::JAE, "jae" },
		{ X86InstructionType::JB, "jb" },
		{ X86InstructionType::JBE, "jbe" },
		{ X86InstructionType::SETE, "sete" },
		{ X86InstructionType::SETNE, "setne" },
		{ X86InstructionType::SETL, "setl" },
		{ X86InstructionType::SETLE, "setle" },
		{ X86InstructionType::SETG, "setg" },
		{ X86InstructionType::SETGE, "setge" },
		{ X86InstructionType::SETA, "seta" },
		{ X86InstructionType::SETAE, "setae" },
		{ X86InstructionType::SETB, "setb" },
		{ X86InstructionType::SETBE, "setbe" },
		{ X86InstructionType::NOP, "nop" },
		{ X86InstructionType::RET, "ret" },
		{ X86InstructionType::LEAVE, "leave" },
	};


	template<typename T>
	concept X86OperandVariant = requires(const T& t)
	{
		{ t.otype() } -> std::same_as<X86OperandType>;
	};

	template<typename T>
	concept X86InstructionVariant = requires(const T& t)
	{
		{ t.itype() } -> std::same_as<X86InstructionType>;
		{ t.emit() } -> std::same_as<ByteRange>;
	};

	template<template<typename> typename Concept, typename... Ts>
		requires (Concept<Ts> && ...)
	using ConstrainedVariant = std::variant<Ts...>;

	using X86Operand = ConstrainedVariant<X86OperandVariant,
		x86::ImmediateOperand,
		x86::IndirectOperand,
		x86::RegisterOperand,
		x86::RegisterIndirectOperand,
		x86::RegisterDisplacedOperand
	>;

	X86OperandType operand_type(const X86Operand& operand)
	{
		return std::visit([]<X86OperandVariant T>(const T& variant)
		{
			return variant.otype();
		}, operand);
	}


	using X86Instruction = ConstrainedVariant<X86InstructionVariant,
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
	>;

	X86InstructionType instruction_type(const X86Instruction& instruction)
	{
		return std::visit([]<X86InstructionVariant T>(const T& variant)
		{
			return variant.itype();
		}, instruction);
	}

	ByteRange emit(const X86Instruction& instruction)
	{
		return std::visit([]<X86InstructionVariant T>(const T& variant)
		{
			return variant.emit();
		}, instruction);
	}


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
			X86OperandType otype() const;
		};
#define imm(val) ImmediateOperand{ val }

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
			X86OperandType otype() const;
		};
#define indirect(addr) IndirectOperand{ addr }

		class RegisterOperand
		{
		public:
			X86Register register_;

		public:
			RegisterOperand(X86Register register_)
				: register_{ register_ }
			{
			}

		public:
			X86OperandType otype() const;
		};
#define reg(reg) RegisterOperand{ reg }

		class RegisterIndirectOperand : public RegisterOperand
		{
		public:
			using RegisterOperand::RegisterOperand;

		public:
			X86OperandType otype() const;
		};
#define reg_indirect(reg) RegisterIndirectOperand{ reg }

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
			X86OperandType otype() const;
		};
#define reg_disp(reg, disp) RegisterDisplacedOperand{ reg, disp }
	}


	namespace x86
	{
		class PushInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			PushInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define push(op) PushInstruction{ op }

		class PopInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			PopInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define pop(op) PopInstruction{ op }

		class MovInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			MovInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define mov(dst, src) MovInstruction{ dst, src }

		class MovzxInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			MovzxInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define movzx(dst, src) MovzxInstruction{ dst, src }

		class AddInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			AddInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define add(dst, src) AddInstruction{ dst, src }

		class SubInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			SubInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define sub(dst, src) SubInstruction{ dst, src }

		class AndInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			AndInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define and(dst, src) AndInstruction{ dst, src }

		class OrInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			OrInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define or(dst, src) OrInstruction{ dst, src }

		class XorInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			XorInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define xor(dst, src) XorInstruction{ dst, src }

		class IncInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			IncInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define inc(op) IncInstruction{ op }

		class DecInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			DecInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define dec(op) DecInstruction{ op }

		class SalInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;
			Offset immediate;

		public:
			SalInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }, immediate{ 1 }
			{
			}

			SalInstruction(std::unique_ptr<X86Operand> operand, Offset immediate)
				: operand{ std::move(operand) }, immediate{ immediate }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define sal(op) SalInstruction{ op }

		class SarInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;
			Offset immediate;

		public:
			SarInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }, immediate{ -1 }
			{
			}

			SarInstruction(std::unique_ptr<X86Operand> operand, Offset immediate)
				: operand{ std::move(operand) }, immediate{ immediate }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define sar(op) SarInstruction{ op }

		class TestInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			TestInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define test(dst, src) TestInstruction{ dst, src }

		class CmpInstruction
		{
		private:
			std::unique_ptr<X86Operand> destination;
			std::unique_ptr<X86Operand> _source;

		public:
			CmpInstruction(std::unique_ptr<X86Operand> destination, std::unique_ptr<X86Operand> source)
				: destination{ std::move(destination) }, _source{ std::move(source) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define cmp(dst, src) CmpInstruction{ dst, src }

		class CallInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			CallInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define call(label, disp) CallInstruction{ label, disp }

		class ApicallInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			ApicallInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define apicall(label, disp) ApicallInstruction{ label, disp }

		class JmpInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JmpInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jmp(label, disp) JmpInstruction{ label, disp }

		class JeInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JeInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define je(label, disp) JeInstruction{ label, disp }

		class JneInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JneInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jne(label, disp) JneInstruction{ label, disp }

		class JlInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JlInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jl(label, disp) JlInstruction{ label, disp }

		class JleInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JleInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jle(label, disp) JleInstruction{ label, disp }

		class JgInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JgInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jg(label, disp) JgInstruction{ label, disp }

		class JgeInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JgeInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jge(label, disp) JgeInstruction{ label, disp }

		class JaInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JaInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define ja(label, disp) JaInstruction{ label, disp }

		class JaeInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JaeInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jae(label, disp) JaeInstruction{ label, disp }

		class JbInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JbInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jb(label, disp) JbInstruction{ label, disp }

		class JbeInstruction
		{
		private:
			const std::string& _label;
			std::int32_t _displacement;

		public:
			JbeInstruction(const std::string& label, std::int32_t displacement)
				: _label{ label }, _displacement{ displacement }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define jbe(label, disp) JbeInstruction{ label, disp }

		class SeteInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SeteInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define sete(op) SeteInstruction{ op }

		class SetneInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetneInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setne(op) SetneInstruction{ op }

		class SetlInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetlInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setl(op) SetlInstruction{ op }

		class SetleInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetleInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setle(op) SetleInstruction{ op }

		class SetgInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetgInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setg(op) SetgInstruction{ op }

		class SetgeInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetgeInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setge(op) SetgeInstruction{ op }

		class SetaInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetaInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define seta(op) SetaInstruction{ op }

		class SetaeInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetaeInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setae(op) SetaeInstruction{ op }

		class SetbInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetbInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setb(op) SetbInstruction{ op }

		class SetbeInstruction
		{
		private:
			std::unique_ptr<X86Operand> operand;

		public:
			SetbeInstruction(std::unique_ptr<X86Operand> operand)
				: operand{ std::move(operand) }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define setbe(op) SetbeInstruction{ op }

		class RetInstruction
		{
		private:
			Offset immediate;

		public:
			RetInstruction()
				: immediate{ -1 }
			{
			}

			RetInstruction(Offset immediate)
				: immediate{ immediate }
			{
			}

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define ret() RetInstruction{}

		class LeaveInstruction
		{
		public:
			using X86Instruction::X86Instruction;

		public:
			X86InstructionType itype() const;
			ByteRange emit() const;
		};
#define leave() LeaveInstruction{}
	}
}

#endif
