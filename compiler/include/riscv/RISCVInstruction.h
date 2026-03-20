#ifndef HAZE_RISCVINSTRUCTION_H
#define HAZE_RISCVINSTRUCTION_H

#include <toolchain/models/Instruction.h>
#include <utility/Constants.h>
#include <utility/PlatformVariables.h>
#include <riscv/RISCVInstructionType.h>
#include <riscv/RISCVLoadInstructionType.h>
#include <riscv/RISCVStoreInstructionType.h>
#include <riscv/RISCVUpperInstructionType.h>
#include <riscv/RISCVSetInstructionType.h>
#include <riscv/RISCVJumpInstructionType.h>
#include <riscv/RISCVArithmeticInstructionType.h>
#include <riscv/RISCVBccInstructionType.h>
#include <riscv/RISCVMExtensionInstructionType.h>
#include <riscv/RISCVAExtensionInstructionType.h>
#include <riscv/RISCVAmoInstructionType.h>
#include <riscv/RISCVZicsrExtensionInstructionType.h>
#include <riscv/RISCVRegister.h>

// Haze RISCVInstruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class RISCVInstruction : public Instruction
	{
	public:
		virtual ArchitectureType ctype() const final override
		{
			return ArchitectureType::RISCV;
		}

	public:
		virtual RISCVInstructionType itype() const = 0;
	};


	class RISCVLoadInstruction : public RISCVInstruction
	{
	protected:
		native_int _immediate;
		RISCVRegister _rd;
		RISCVRegister _rs1;

	public:
		RISCVLoadInstruction(native_int immediate, RISCVRegister rd, RISCVRegister rs1)
			: _immediate{ immediate }, _rd{ rd }, _rs1{ rs1 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVLoadInstructionType ltype() const = 0;
	};

	class RISCVLbInstruction : public RISCVLoadInstruction
	{
	public:
		using RISCVLoadInstruction::RISCVLoadInstruction;

	public:
		virtual RISCVLoadInstructionType ltype() const final override;
		virtual byterange emit() const final override;
	};
#define lb(rd, imm, rs1) RISCVLbInstruction{ imm, rd, rs1 }

	class RISCVLhInstruction : public RISCVLoadInstruction
	{
	public:
		using RISCVLoadInstruction::RISCVLoadInstruction;

	public:
		virtual RISCVLoadInstructionType ltype() const final override;
		virtual byterange emit() const final override;
	};
#define lh(rd, imm, rs1) RISCVLhInstruction{ imm, rd, rs1 }

	class RISCVLwInstruction : public RISCVLoadInstruction
	{
	public:
		using RISCVLoadInstruction::RISCVLoadInstruction;

	public:
		virtual RISCVLoadInstructionType ltype() const final override;
		virtual byterange emit() const final override;
	};
#define lw(rd, imm, rs1) RISCVLwInstruction{ imm, rd, rs1 }

	class RISCVLbuInstruction : public RISCVLoadInstruction
	{
	public:
		using RISCVLoadInstruction::RISCVLoadInstruction;

	public:
		virtual RISCVLoadInstructionType ltype() const final override;
		virtual byterange emit() const final override;
	};
#define lbu(rd, imm, rs1) RISCVLbuInstruction{ imm, rd, rs1 }

	class RISCVLhuInstruction : public RISCVLoadInstruction
	{
	public:
		using RISCVLoadInstruction::RISCVLoadInstruction;

	public:
		virtual RISCVLoadInstructionType ltype() const final override;
		virtual byterange emit() const final override;
	};
#define lhu(rd, imm, rs1) RISCVLhuInstruction{ imm, rd, rs1 }


	class RISCVStoreInstruction : public RISCVInstruction
	{
	protected:
		native_int _immediate;
		RISCVRegister _rs1;
		RISCVRegister _rs2;

	public:
		RISCVStoreInstruction(native_int immediate, RISCVRegister rs1, RISCVRegister rs2)
			: _immediate{ immediate }, _rs1{ rs1 }, _rs2{ rs2 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVStoreInstructionType stype() const = 0;
	};

	class RISCVSbInstruction : public RISCVStoreInstruction
	{
	public:
		using RISCVStoreInstruction::RISCVStoreInstruction;

	public:
		virtual byterange emit() const final override;
		virtual RISCVStoreInstructionType stype() const final override;
	};
#define sb(rs2, imm, rs1) RISCVSbInstruction{ imm, rs1, rs2 }

	class RISCVShInstruction : public RISCVStoreInstruction
	{
	public:
		using RISCVStoreInstruction::RISCVStoreInstruction;

	public:
		virtual byterange emit() const final override;
		virtual RISCVStoreInstructionType stype() const final override;
	};
#define sh(rs2, imm, rs1) RISCVShInstruction{ imm, rs1, rs2 }

	class RISCVSwInstruction : public RISCVStoreInstruction
	{
	public:
		using RISCVStoreInstruction::RISCVStoreInstruction;

	public:
		virtual byterange emit() const final override;
		virtual RISCVStoreInstructionType stype() const final override;
	};
#define sw(rs2, imm, rs1) RISCVSwInstruction{ imm, rs1, rs2 }


	class RISCVUpperInstruction : public RISCVInstruction
	{
	protected:
		native_int _immediate;
		RISCVRegister _rd;

	public:
		RISCVUpperInstruction(native_int immediate, RISCVRegister rd)
			: _immediate{ immediate }, _rd{ rd }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVUpperInstructionType utype() const = 0;
	};

	class RISCVLuiInstruction : public RISCVUpperInstruction
	{
	public:
		using RISCVUpperInstruction::RISCVUpperInstruction;

	public:
		virtual RISCVUpperInstructionType utype() const final override;
		virtual byterange emit() const final override;
	};
#define lui(rd, imm) RISCVLuiInstruction{ imm, rd }

	class RISCVAuipcInstruction : public RISCVUpperInstruction
	{
	public:
		using RISCVUpperInstruction::RISCVUpperInstruction;

	public:
		virtual RISCVUpperInstructionType utype() const final override;
		virtual byterange emit() const final override;
	};
#define auipc(rd, imm) RISCVAuipcInstruction{ imm, rd }


	class RISCVArithmeticInstruction : public RISCVInstruction
	{
	protected:
		RISCVRegister _rd;
		RISCVRegister _rs1;

	public:
		RISCVArithmeticInstruction(RISCVRegister rd, RISCVRegister rs1)
			: _rd{ rd }, _rs1{ rs1 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVArithmeticInstructionType mtype() const = 0;
	};

	class RISCVRegisterArithmeticInstruction : public RISCVArithmeticInstruction
	{
	protected:
		RISCVRegister _rs2;

	public:
		RISCVRegisterArithmeticInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
			: RISCVArithmeticInstruction{ rd, rs1 }, _rs2{ rs2 }
		{
		}
	};

	class RISCVAddInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define add(rd, rs1, rs2) RISCVAddInstruction{ rd, rs1, rs2 }

	class RISCVSubInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define sub(rd, rs1, rs2) RISCVSubInstruction{ rd, rs1, rs2 }

	class RISCVAndInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define and(rd, rs1, rs2) RISCVAndInstruction{ rd, rs1, rs2 }

	class RISCVOrInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define or(rd, rs1, rs2) RISCVOrInstruction{ rd, rs1, rs2 }

	class RISCVXorInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define xor(rd, rs1, rs2) RISCVXorInstruction{ rd, rs1, rs2 }

	class RISCVSllInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define sll(rd, rs1, rs2) RISCVSllInstruction{ rd, rs1, rs2 }

	class RISCVSrlInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define srl(rd, rs1, rs2) RISCVSrlInstruction{ rd, rs1, rs2 }

	class RISCVSraInstruction : public RISCVRegisterArithmeticInstruction
	{
	public:
		using RISCVRegisterArithmeticInstruction::RISCVRegisterArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define sra(rd, rs1, rs2) RISCVSraInstruction{ rd, rs1, rs2 }

	class RISCVImmediateArithmeticInstruction : public RISCVArithmeticInstruction
	{
	protected:
		native_int _immediate;

	public:
		RISCVImmediateArithmeticInstruction(RISCVRegister rd, RISCVRegister rs1, native_int immediate)
			: RISCVArithmeticInstruction{ rd, rs1 }, _immediate{ immediate }
		{
		}
	};

	class RISCVAddiInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define addi(rd, rs1, imm) RISCVAddiInstruction{ rd, rs1, imm }

	class RISCVAndiInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define andi(rd, rs1, imm) RISCVAndiInstruction{ rd, rs1, imm }

	class RISCVOriInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define ori(rd, rs1, imm) RISCVOriInstruction{ rd, rs1, imm }

	class RISCVXoriInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define xori(rd, rs1, imm) RISCVXoriInstruction{ rd, rs1, imm }

	class RISCVSlliInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define slli(rd, rs1, imm) RISCVSlliInstruction{ rd, rs1, imm }

	class RISCVSrliInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define srli(rd, rs1, imm) RISCVSrliInstruction{ rd, rs1, imm }

	class RISCVSraiInstruction : public RISCVImmediateArithmeticInstruction
	{
	public:
		using RISCVImmediateArithmeticInstruction::RISCVImmediateArithmeticInstruction;

	public:
		virtual RISCVArithmeticInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define srai(rd, rs1, imm) RISCVSraiInstruction{ rd, rs1, imm }


	class RISCVSetInstruction : public RISCVInstruction
	{
	protected:
		native_int _immediate;
		RISCVRegister _rd;
		RISCVRegister _rs1;

	public:
		RISCVSetInstruction(native_int immediate, RISCVRegister rd, RISCVRegister rs1)
			: _immediate{ immediate }, _rd{ rd }, _rs1{ rs1 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVSetInstructionType stype() const = 0;
	};

	class RISCVSltiInstruction : public RISCVSetInstruction
	{
	public:
		using RISCVSetInstruction::RISCVSetInstruction;

	public:
		virtual RISCVSetInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};
#define slti(rd, rs1, imm) RISCVSltiInstruction{ imm, rd, rs1 }

	class RISCVSltInstruction : public RISCVSetInstruction
	{
	public:
		using RISCVSetInstruction::RISCVSetInstruction;

	public:
		virtual RISCVSetInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};
#define slt(rd, rs1, imm) RISCVSltInstruction{ imm, rd, rs1 }

	class RISCVSltiuInstruction : public RISCVSetInstruction
	{
	public:
		using RISCVSetInstruction::RISCVSetInstruction;

	public:
		virtual RISCVSetInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};
#define sltiu(rd, rs1, imm) RISCVSltiuInstruction{ imm, rd, rs1 }

	class RISCVSltuInstruction : public RISCVSetInstruction
	{
	public:
		using RISCVSetInstruction::RISCVSetInstruction;

	public:
		virtual RISCVSetInstructionType stype() const final override;
		virtual byterange emit() const final override;
	};
#define slti(rd, rs1, imm) RISCVSltiInstruction{ imm, rd, rs1 }


	class RISCVJumpInstruction : public RISCVInstruction
	{
	protected:
		native_int _immediate;
		RISCVRegister _rd;

	public:
		RISCVJumpInstruction(native_int immediate, RISCVRegister rd)
			: _immediate{ immediate }, _rd{ rd }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVJumpInstructionType jtype() const = 0;
	};

	class RISCVJalInstruction : public RISCVJumpInstruction
	{
	public:
		using RISCVJumpInstruction::RISCVJumpInstruction;

	public:
		virtual RISCVJumpInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};
#define jal(rd, imm) RISCVJalInstruction{ imm, rd }

	class RISCVJalrInstruction : public RISCVJumpInstruction
	{
	private:
		RISCVRegister _rs1;

	public:
		RISCVJalrInstruction(native_int immediate, RISCVRegister rd, RISCVRegister rs1)
			: RISCVJumpInstruction{ immediate, rd }, _rs1{ rs1 }
		{
		}

	public:
		virtual RISCVJumpInstructionType jtype() const final override;
		virtual byterange emit() const final override;
	};
#define jalr(rd, imm, rs1) RISCVJalrInstruction{ imm, rd, rs1 }



	class RISCVBccInstruction : public RISCVInstruction
	{
	protected:
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
#define beq(rs1, rs2, imm) RISCVBeqInstruction{ imm, rs1, rs2 }

	class RISCVBneInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};
#define bne(rs1, rs2, imm) RISCVBneInstruction{ imm, rs1, rs2 }

	class RISCVBltInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};
#define blt(rs1, rs2, imm) RISCVBltInstruction{ imm, rs1, rs2 }

	class RISCVBgeInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};
#define bge(rs1, rs2, imm) RISCVBgeInstruction{ imm, rs1, rs2 }

	class RISCVBltuInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};
#define bltu(rs1, rs2, imm) RISCVBltuInstruction{ imm, rs1, rs2 }

	class RISCVBgeuInstruction : public RISCVBccInstruction
	{
	public:
		using RISCVBccInstruction::RISCVBccInstruction;

	public:
		virtual RISCVBccInstructionType btype() const final override;
		virtual byterange emit() const final override;
	};
#define bgeu(rs1, rs2, imm) RISCVBgeuInstruction{ imm, rs1, rs2 }


	class RISCVNopInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define nop() RISCVNopInstruction{}

	class RISCVEcallInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define ecall() RISCVEcallInstruction{}

	class RISCVEbreakInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define ebreak() RISCVEbreakInstruction{}

	class RISCVWfiInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define wfi() RISCVWfiInstruction{}

	class RISCVFenceInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define fence() RISCVFenceInstruction{}


	class RISCVMExtensionInstruction : public RISCVInstruction
	{
	protected:
		RISCVRegister _rd;
		RISCVRegister _rs1;
		RISCVRegister _rs2;

	public:
		RISCVMExtensionInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
			: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVMExtensionInstructionType mtype() const = 0;
	};

	class RISCVMulInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;

	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define mul(rd, rs1, rs2) RISCVMulInstruction{ rd, rs1, rs2 }
	
	class RISCVMulhInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;

	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define mulh(rd, rs1, rs2) RISCVMulhInstruction{ rd, rs1, rs2 }

	class RISCVMulhuInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;

	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define mulhu(rd, rs1, rs2) RISCVMulhuInstruction{ rd, rs1, rs2 }

	class RISCVMulhsuInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;

	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define mulhsu(rd, rs1, rs2) RISCVMulhsuInstruction{ rd, rs1, rs2 }

	class RISCVDivInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;
	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define div(rd, rs1, rs2) RISCVDivInstruction{ rd, rs1, rs2 }

	class RISCVDivuInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;

	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define divu(rd, rs1, rs2) RISCVDivuInstruction{ rd, rs1, rs2 }

	class RISCVRemInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;
	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define rem(rd, rs1, rs2) RISCVRemInstruction{ rd, rs1, rs2 }

	class RISCVRemuInstruction : public RISCVMExtensionInstruction
	{
	public:
		using RISCVMExtensionInstruction::RISCVMExtensionInstruction;
	public:
		virtual RISCVMExtensionInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define remu(rd, rs1, rs2) RISCVRemuInstruction{ rd, rs1, rs2 }



	class RISCVAExtensionInstruction : public RISCVInstruction
	{
	protected:
		RISCVRegister _rd;
		RISCVRegister _rs1;
		bool _acquire;
		bool _release;

	public:
		RISCVAExtensionInstruction(RISCVRegister rd, RISCVRegister rs1, bool acquire, bool release)
			: _rd{ rd }, _rs1{ rs1 }, _acquire{ acquire }, _release{ release }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVAExtensionInstructionType atype() const = 0;
	};

	class RISCVLrwInstruction : public RISCVAExtensionInstruction
	{
	public:
		using RISCVAExtensionInstruction::RISCVAExtensionInstruction;

	public:
		virtual RISCVAExtensionInstructionType atype() const final override;
		virtual byterange emit() const final override;
	};
#define lrw(rd, rs1, acquire, release) RISCVLrwInstruction{ rd, rs1, acquire, release }

	class RISCVScwInstruction : public RISCVAExtensionInstruction
	{
	protected:
		RISCVRegister _rs2;

	public:
		RISCVScwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
			: RISCVAExtensionInstruction{ rd, rs1, acquire, release }, _rs2{ rs2 }
		{
		}

	public:
		virtual RISCVAExtensionInstructionType atype() const final override;
		virtual byterange emit() const final override;
	};
#define scw(rd, rs2, rs1, acquire, release) RISCVScwInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmoInstruction : public RISCVAExtensionInstruction
	{
	protected:
		RISCVRegister _rs2;

	public:
		RISCVAmoInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
			: RISCVAExtensionInstruction{ rd, rs1, acquire, release }, _rs2{ rs2 }
		{
		}

	public:
		virtual RISCVAExtensionInstructionType atype() const final override;
		virtual RISCVAmoInstructionType mtype() const = 0;
	};

	class RISCVAmoswapwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amoswapw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmoaddwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amoaddw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmoandwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amoandw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmoorwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amoorw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmoxorwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amoxorw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmominwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amominw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmomaxwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amomaxw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmominuwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amominuw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }

	class RISCVAmomaxuwInstruction : public RISCVAmoInstruction
	{
	public:
		using RISCVAmoInstruction::RISCVAmoInstruction;

	public:
		virtual RISCVAmoInstructionType mtype() const final override;
		virtual byterange emit() const final override;
	};
#define amomaxuw(rd, rs2, rs1, acquire, release) RISCVAmoInstruction{ rd, rs1, rs2, acquire, release }


	// C extension instructions
	namespace riscv
	{
		class ClwspInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _immediate;

		public:
			ClwspInstruction(RISCVRegister rd, native_int immediate)
				: _rd{ rd }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define clwsp(rd, imm) ClwspInstruction{ rd, imm }

		class CswspInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rs2;
			native_int _immediate;

		public:
			CswspInstruction(RISCVRegister rs2, native_int immediate)
				: _rs2{ rs2 }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cswsp(rs2, imm) CswspInstruction{ rs2, imm }

		class ClwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			native_int _immediate;

		public:
			ClwInstruction(RISCVRegister rd, native_int immediate, RISCVRegister rs1)
				: _rd{ rd }, _rs1{ rs1 }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define clw(rd, imm, rs1) ClwInstruction{ rd, imm, rs1 }

		class CswInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			native_int _immediate;

		public:
			CswInstruction(RISCVRegister rs2, native_int immediate, RISCVRegister rs1)
				: _rs1{ rs1 }, _rs2{ rs2 }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define csw(rs2, imm, rs1) CswInstruction{ rs2, imm, rs1 }

		class CjInstruction : public RISCVInstruction
		{
		private:
			native_int _immediate;

		public:
			CjInstruction(native_int immediate)
				: _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cj(imm) CjInstruction{ imm }

		class CjalInstruction : public RISCVInstruction
		{
		private:
			native_int _immediate;

		public:
			CjalInstruction(native_int immediate)
				: _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cjal(imm) CjalInstruction{ imm }

		class CjrInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rs1;

		public:
			CjrInstruction(RISCVRegister rs1)
				: _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cjr(rs1) CjrInstruction{ rs1 }

		class CjalrInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rs1;

		public:
			CjalrInstruction(RISCVRegister rs1)
				: _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cjalr(rs1) CjalrInstruction{ rs1 }

		class CbeqzInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rs2;
			native_int _immediate;

		public:
			CbeqzInstruction(RISCVRegister rs2, native_int immediate)
				: _rs2{ rs2 }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cbeqz(rs2, imm) CbeqzInstruction{ rs2, imm }

		class CbnezInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rs2;
			native_int _immediate;

		public:
			CbnezInstruction(RISCVRegister rs2, native_int immediate)
				: _rs2{ rs2 }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cbnez(rs2, imm) CbnezInstruction{ rs2, imm }

		class CliInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _immediate;

		public:
			CliInstruction(RISCVRegister rd, native_int immediate)
				: _rd{ rd }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cli(rd, imm) CliInstruction{ rd, imm }

		class CluiInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _immediate;

		public:
			CluiInstruction(RISCVRegister rd, native_int immediate)
				: _rd{ rd }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define clui(rd, imm) CluiInstruction{ rd, imm }

		class CaddiInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _immediate;

		public:
			CaddiInstruction(RISCVRegister rd, native_int immediate)
				: _rd{ rd }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define caddi(rd, imm) CaddiInstruction{ rd, imm }

		class Caddi16spInstruction : public RISCVInstruction
		{
		private:
			native_int _immediate;

		public:
			Caddi16spInstruction(native_int immediate)
				: _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define caddi16sp(imm) Caddi16spInstruction{ imm }

		class Caddi4spnInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _immediate;

		public:
			Caddi4spnInstruction(RISCVRegister rd, native_int immediate)
				: _rd{ rd }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define caddi4spn(rd, imm) Caddi4spnInstruction{ rd, imm }

		class CslliInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _shamt;

		public:
			CslliInstruction(RISCVRegister rd, native_int shamt)
				: _rd{ rd }, _shamt{ shamt }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cslli(rd, shamt) CslliInstruction{ rd, shamt }

		class CsrliInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _shamt;

		public:
			CsrliInstruction(RISCVRegister rd, native_int shamt)
				: _rd{ rd }, _shamt{ shamt }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define csrli(rd, shamt) CsrliInstruction{ rd, shamt }

		class CsraiInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _shamt;

		public:
			CsraiInstruction(RISCVRegister rd, native_int shamt)
				: _rd{ rd }, _shamt{ shamt }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define csrai(rd, shamt) CsraiInstruction{ rd, shamt }

		class CandiInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			native_int _immediate;

		public:
			CandiInstruction(RISCVRegister rd, native_int immediate)
				: _rd{ rd }, _immediate{ immediate }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define candi(rd, imm) CandiInstruction{ rd, imm }

		class CmvInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs2;

		public:
			CmvInstruction(RISCVRegister rd, RISCVRegister rs2)
				: _rd{ rd }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cmv(rd, rs2) CmvInstruction{ rd, rs2 }

		class CaddInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs2;

		public:
			CaddInstruction(RISCVRegister rd, RISCVRegister rs2)
				: _rd{ rd }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cadd(rd, rs2) CaddInstruction{ rd, rs2 }

		class CsubInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs2;

		public:
			CsubInstruction(RISCVRegister rd, RISCVRegister rs2)
				: _rd{ rd }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define csub(rd, rs2) CsubInstruction{ rd, rs2 }

		class CxorInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs2;

		public:
			CxorInstruction(RISCVRegister rd, RISCVRegister rs2)
				: _rd{ rd }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cxor(rd, rs2) CxorInstruction{ rd, rs2 }

		class CorInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs2;

		public:
			CorInstruction(RISCVRegister rd, RISCVRegister rs2)
				: _rd{ rd }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cor(rd, rs2) CorInstruction{ rd, rs2 }

		class CandInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs2;

		public:
			CandInstruction(RISCVRegister rd, RISCVRegister rs2)
				: _rd{ rd }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cand(rd, rs2) CandInstruction{ rd, rs2 }

		class CnopInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cnop() CnopInstruction{}

		class CebreakInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual byterange emit() const final override;
		};
#define cebreak() CebreakInstruction{}
	}


	class RISCVZicsrExtensionInstruction : public RISCVInstruction
	{
	protected:
		native_int _csr;
		RISCVRegister _rd;

	public:
		RISCVZicsrExtensionInstruction(native_int csr, RISCVRegister rd)
			: _csr{ csr }, _rd{ rd }
		{
		}

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual RISCVZicsrExtensionInstructionType ztype() const = 0;
	};

	class RISCVRegisterZicsrExtensionInstruction : public RISCVZicsrExtensionInstruction
	{
	protected:
		RISCVRegister _rs1;

	public:
		RISCVRegisterZicsrExtensionInstruction(native_int csr, RISCVRegister rd, RISCVRegister rs1)
			: RISCVZicsrExtensionInstruction{ csr, rd }, _rs1{ rs1 }
		{
		}
	};

	class RISCVCssrwInstruction : public RISCVRegisterZicsrExtensionInstruction
	{
	public:
		using RISCVRegisterZicsrExtensionInstruction::RISCVRegisterZicsrExtensionInstruction;

	public:
		virtual RISCVZicsrExtensionInstructionType ztype() const final override;
		virtual byterange emit() const final override;
	};
#define csrrw(rd, csr, rs1) RISCVCssrwInstruction{ csr, rd, rs1 }

	class RISCVCsrrsInstruction : public RISCVRegisterZicsrExtensionInstruction
	{
	public:
		using RISCVRegisterZicsrExtensionInstruction::RISCVRegisterZicsrExtensionInstruction;
	public:
		virtual RISCVZicsrExtensionInstructionType ztype() const final override;
		virtual byterange emit() const final override;
	};
#define csrrs(rd, csr, rs1) RISCVCsrrsInstruction{ csr, rd, rs1 }

	class RISCVCsrrcInstruction : public RISCVRegisterZicsrExtensionInstruction
	{
	public:
		using RISCVRegisterZicsrExtensionInstruction::RISCVRegisterZicsrExtensionInstruction;
	public:
		virtual RISCVZicsrExtensionInstructionType ztype() const final override;
		virtual byterange emit() const final override;
	};
#define csrrc(rd, csr, rs1) RISCVCsrrcInstruction{ csr, rd, rs1 }

	class RISCVImmediateZicsrExtensionInstruction : public RISCVZicsrExtensionInstruction
	{
	protected:
		native_int _immediate;

	public:
		RISCVImmediateZicsrExtensionInstruction(native_int csr, RISCVRegister rd, native_int immediate)
			: RISCVZicsrExtensionInstruction{ csr, rd }, _immediate{ immediate }
		{
		}
	};

	class RISCVCsrrwiInstruction : public RISCVImmediateZicsrExtensionInstruction
	{
	public:
		using RISCVImmediateZicsrExtensionInstruction::RISCVImmediateZicsrExtensionInstruction;

	public:
		virtual RISCVZicsrExtensionInstructionType ztype() const final override;
		virtual byterange emit() const final override;
	};
#define csrrwi(rd, csr, imm) RISCVCsrrwiInstruction{ csr, rd, imm }

	class RISCVCsrrsiInstruction : public RISCVImmediateZicsrExtensionInstruction
	{
	public:
		using RISCVImmediateZicsrExtensionInstruction::RISCVImmediateZicsrExtensionInstruction;
	public:
		virtual RISCVZicsrExtensionInstructionType ztype() const final override;
		virtual byterange emit() const final override;
	};
#define csrrsi(rd, csr, imm) RISCVCsrrsiInstruction{ csr, rd, imm }

	class RISCVCsrrciInstruction : public RISCVImmediateZicsrExtensionInstruction
	{
	public:
		using RISCVImmediateZicsrExtensionInstruction::RISCVImmediateZicsrExtensionInstruction;
	public:
		virtual RISCVZicsrExtensionInstructionType ztype() const final override;
		virtual byterange emit() const final override;
	};
#define csrrci(rd, csr, imm) RISCVCsrrciInstruction{ csr, rd, imm }
	

	class RISCVFenceiInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define fencei() RISCVFenceiInstruction{}


	class RISCVUretInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;

	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define uret() RISCVUretInstruction{}

	class RISCVSretInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;
	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define sret() RISCVSretInstruction{}

	class RISCVMretInstruction : public RISCVInstruction
	{
	public:
		using RISCVInstruction::RISCVInstruction;
	public:
		virtual RISCVInstructionType itype() const final override;
		virtual byterange emit() const final override;
	};
#define mret() RISCVMretInstruction{}

}

#endif
