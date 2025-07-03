#ifndef HAZE_X86INSTRUCTION_H
#define HAZE_X86INSTRUCTION_H

#include "X86InstructionType.h"
#include "X86Operand.h"
#include "X86JccInstructionType.h"
#include "X86SetccInstructionType.h"
#include "Constants.h"

// Haze X86Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Instruction
	{
	public:
		virtual X86InstructionType itype() const = 0;
		virtual byterange emit() const = 0;
	};

	class X86PushInstruction : public X86Instruction
	{
	private:
		X86Operand* _operand;

	public:
		X86PushInstruction(X86Operand* operand)
			: _operand{ operand }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86PopInstruction : public X86Instruction
	{
	private:
		X86Operand* _operand;

	public:
		X86PopInstruction(X86Operand* operand)
			: _operand{ operand }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86MovInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86MovInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86MovzxInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86MovzxInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86AddInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86AddInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SubInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86SubInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86AndInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86AndInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86OrInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86OrInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86XorInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86XorInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86IncInstruction : public X86Instruction
	{
	private:
		X86Operand* _operand;

	public:
		X86IncInstruction(X86Operand* operand)
			: _operand{ operand }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86DecInstruction : public X86Instruction
	{
	private:
		X86Operand* _operand;

	public:
		X86DecInstruction(X86Operand* operand)
			: _operand{ operand }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86TestInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86TestInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86CmpInstruction : public X86Instruction
	{
	private:
		X86Operand* _destination;
		X86Operand* _source;

	public:
		X86CmpInstruction(X86Operand* destination, X86Operand* source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JccInstruction : public X86Instruction
	{
	protected:
		const std::string& _label;
		std::int32_t _displacement;

	public:
		X86JccInstruction(const std::string& label, std::int32_t displacement)
			: _label{ label }, _displacement{ displacement }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual X86JccInstructionType jtype() const = 0;
	};

	class X86CallInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86ApiCallInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JmpInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JeInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JneInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JlInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JleInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JgInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JgeInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JaInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JaeInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JbInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86JbeInstruction : public X86JccInstruction
	{
	public:
		using X86JccInstruction::X86JccInstruction;

	public:
		virtual X86JccInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetccInstruction : public X86Instruction
	{
	protected:
		X86Operand* _operand;
		
	public:
		X86SetccInstruction(X86Operand* operand)
			: _operand{ operand }
		{
		}

	public:
		virtual X86InstructionType itype() const final override;
		virtual X86SetccInstructionType stype() const = 0;
	};

	class X86SeteInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetneInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetlInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetleInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetgInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetgeInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetaInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetaeInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetbInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};

	class X86SetbeInstruction : public X86SetccInstruction
	{
	public:
		using X86SetccInstruction::X86SetccInstruction;

	public:
		virtual X86SetccInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};
}

#endif
