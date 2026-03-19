#ifndef HAZE_INSTRUCTIONTYPE_H
#define HAZE_INSTRUCTIONTYPE_H

#include <toolchain/models/Instruction.h>
#include <utility/Constants.h>
#include <utility/PlatformVariables.h>
#include <riscv/RISCVWidthType.h>
#include <riscv/RISCVInstructionType.h>
#include <riscv/RISCVSignednessType.h>
#include <riscv/RISCVBccInstructionType.h>
#include <riscv/RISCVRegister.h>

// Haze RISCVInstruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class RISCVInstruction : public Instruction<RISCVInstructionType>
	{
	public:
		virtual ArchitectureType atype() const final override
		{
			return ArchitectureType::RISCV;
		}
	};

	class RISCVLoadInstruction : public RISCVInstruction
	{
	private:
		RISCVWidthType _width;
		RISCVSignednessType _signedness;
		RISCVRegister _rd;
		RISCVRegister _rs1;

	public:
		RISCVLoadInstruction(RISCVWidthType width, RISCVSignednessType signedness, RISCVRegister rd, RISCVRegister rs1)
			: _width{ width }, _signedness{ signedness }, _rd{ rd }, _rs1{ rs1 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVStoreInstruction : public RISCVInstruction
	{
	private:
		RISCVWidthType _width;
		RISCVRegister _rs1;
		RISCVRegister _rs2;

	public:
		RISCVStoreInstruction(RISCVWidthType width, RISCVRegister rs1, RISCVRegister rs2)
			: _width{ width }, _rs1{ rs1 }, _rs2{ rs2 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVSbInstruction : public RISCVStoreInstruction
	{
	public:
		RISCVSbInstruction(RISCVRegister rs1, RISCVRegister rs2)
			: RISCVStoreInstruction{ RISCVWidthType::BYTE, rs1, rs2 }
		{
		}

#error TODO FINISH THIS STUFF!
	};


	class RISCVBccInstruction : public RISCVInstruction
	{
	private:
		native_int _immediate;
		RISCVRegister _rs1;
		RISCVRegister _rs2;

	public:
		RISCVBccInstruction(native_int immediate, RISCVRegister rs1, RISCVRegister rs2)
			: _immediate{ immediate }, _rs1{ rs1 }, _rs2{ rs2 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVBccInstructionType btype() const = 0;
	};

	class RISCVBeqInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	}; 

	class RISCVBneInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVBltInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVBgeInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVBltuInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVBgeuInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};


	class RISCVNopInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVEcallInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVEbreakInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVWfiInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

	class RISCVFenceInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};

}

#endif
