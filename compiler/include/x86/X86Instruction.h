#ifndef HAZE_X86INSTRUCTION_H
#define HAZE_X86INSTRUCTION_H

#include <toolchain/models/Instruction.h>
#include <utility/Constants.h>
#include <x86/X86InstructionType.h>
#include <x86/X86Operand.h>

// Haze X86Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Instruction : public Instruction
	{
	public:
		virtual ArchitectureType ctype() const final override
		{
			return ArchitectureType::X86;
		}

	public:
		virtual X86InstructionType itype() const = 0;
	};

	namespace x86
	{
		class PushInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			PushInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define push(op) PushInstruction{ op }

		class PopInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			PopInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define pop(op) PopInstruction{ op }

		class MovInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define mov(dst, src) MovInstruction{ dst, src }

		class MovzxInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define movzx(dst, src) MovzxInstruction{ dst, src }

		class AddInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define add(dst, src) AddInstruction{ dst, src }

		class SubInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sub(dst, src) SubInstruction{ dst, src }

		class AndInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define and(dst, src) AndInstruction{ dst, src }

		class OrInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define or(dst, src) OrInstruction{ dst, src }

		class XorInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define xor(dst, src) XorInstruction{ dst, src }

		class IncInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			IncInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define inc(op) IncInstruction{ op }

		class DecInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			DecInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define dec(op) DecInstruction{ op }

		class SalInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;
			Offset immediate;

		public:
			SalInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }, immediate{ 1 }
			{
			}

			SalInstruction(std::unique_ptr<X86Operand> operand, Offset immediate)
				: _operand{ std::move(operand) }, immediate{ immediate }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sal(op) SalInstruction{ op }

		class SarInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;
			Offset immediate;

		public:
			SarInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }, immediate{ -1 }
			{
			}

			SarInstruction(std::unique_ptr<X86Operand> operand, Offset immediate)
				: _operand{ std::move(operand) }, immediate{ immediate }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sar(op) SarInstruction{ op }

		class TestInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define test(dst, src) TestInstruction{ dst, src }

		class CmpInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define cmp(dst, src) CmpInstruction{ dst, src }

		class CallInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define call(label, disp) CallInstruction{ label, disp }

		class ApicallInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define apicall(label, disp) ApicallInstruction{ label, disp }

		class JmpInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jmp(label, disp) JmpInstruction{ label, disp }

		class JeInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define je(label, disp) JeInstruction{ label, disp }

		class JneInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jne(label, disp) JneInstruction{ label, disp }

		class JlInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jl(label, disp) JlInstruction{ label, disp }

		class JleInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jle(label, disp) JleInstruction{ label, disp }

		class JgInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jg(label, disp) JgInstruction{ label, disp }

		class JgeInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jge(label, disp) JgeInstruction{ label, disp }

		class JaInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define ja(label, disp) JaInstruction{ label, disp }

		class JaeInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jae(label, disp) JaeInstruction{ label, disp }

		class JbInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jb(label, disp) JbInstruction{ label, disp }

		class JbeInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jbe(label, disp) JbeInstruction{ label, disp }

		class SeteInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SeteInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sete(op) SeteInstruction{ op }

		class SetneInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetneInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setne(op) SetneInstruction{ op }

		class SetlInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetlInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setl(op) SetlInstruction{ op }

		class SetleInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetleInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setle(op) SetleInstruction{ op }

		class SetgInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetgInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setg(op) SetgInstruction{ op }

		class SetgeInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetgeInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setge(op) SetgeInstruction{ op }

		class SetaInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetaInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define seta(op) SetaInstruction{ op }

		class SetaeInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetaeInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setae(op) SetaeInstruction{ op }

		class SetbInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetbInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setb(op) SetbInstruction{ op }

		class SetbeInstruction : public X86Instruction
		{
		private:
			std::unique_ptr<X86Operand> _operand;

		public:
			SetbeInstruction(std::unique_ptr<X86Operand> operand)
				: _operand{ std::move(operand) }
			{
			}

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define setbe(op) SetbeInstruction{ op }

		class RetInstruction : public X86Instruction
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
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define ret() RetInstruction{}

		class LeaveInstruction : public X86Instruction
		{
		public:
			using X86Instruction::X86Instruction;

		public:
			virtual X86InstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define leave() LeaveInstruction{}
	}
}

#endif
