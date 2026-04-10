#ifndef HAZE_RISCVINSTRUCTION_H
#define HAZE_RISCVINSTRUCTION_H

#include <toolchain/models/Instruction.h>
#include <utility/Constants.h>
#include <utility/PlatformVariables.h>
#include <riscv/RISCVInstructionType.h>
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


	// I base instruction set instructions
	namespace riscv
	{
		class LbInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			LbInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lb(rd, imm, rs1) LbInstruction{ imm, rd, rs1 }

		class LhInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			LhInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lh(rd, imm, rs1) LhInstruction{ imm, rd, rs1 }

		class LwInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;
		public:
			LwInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lw(rd, imm, rs1) LwInstruction{ imm, rd, rs1 }

		class LbuInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;
		public:
			LbuInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lbu(rd, imm, rs1) LbuInstruction{ imm, rd, rs1 }

		class LhuInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;
		public:
			LhuInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lhu(rd, imm, rs1) LhuInstruction{ imm, rd, rs1 }
	
		class SbInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs2;
			RISCVRegister _rs1;

		public:
			SbInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sb(rs2, imm, rs1) SbInstruction{ imm, rs1, rs2 }

		class ShInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs2;
			RISCVRegister _rs1;
		public:
			ShInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sh(rs2, imm, rs1) ShInstruction{ imm, rs1, rs2 }

		class SwInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs2;
			RISCVRegister _rs1;
		public:
			SwInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sw(rs2, imm, rs1) SwInstruction{ imm, rs1, rs2 }

		class LuiInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;

		public:
			LuiInstruction(native_int imm, RISCVRegister rd)
				: _imm{ imm }, _rd{ rd }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lui(rd, imm) LuiInstruction{ imm, rd }

		class AuipcInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
		public:
			AuipcInstruction(native_int imm, RISCVRegister rd)
				: _imm{ imm }, _rd{ rd }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define auipc(rd, imm) AuipcInstruction{ imm, rd }

		class AddInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			AddInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define add(rd, rs1, rs2) AddInstruction{ rd, rs1, rs2

		class SubInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			SubInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sub(rd, rs1, rs2) SubInstruction{ rd, rs1, rs2 }

		class AndInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			AndInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define and(rd, rs1, rs2) AndInstruction{ rd, rs1, rs2 }

		class OrInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			OrInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define or(rd, rs1, rs2) OrInstruction{ rd, rs1, rs2 }

		class XorInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			XorInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define xor(rd, rs1, rs2) XorInstruction{ rd, rs1, rs2 }

		class SllInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			SllInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sll(rd, rs1, rs2) SllInstruction{ rd, rs1, rs2 }

		class SrlInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			SrlInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define srl(rd, rs1, rs2) SrlInstruction{ rd, rs1, rs2 }

		class SraInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			SraInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sra(rd, rs1, rs2) SraInstruction{ rd, rs1, rs2 }

		class AddiInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			AddiInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define addi(rd, rs1, imm) AddiInstruction{ imm, rd, rs1 }

		class AndiInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			AndiInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define andi(rd, rs1, imm) AndiInstruction{ imm, rd, rs1 }

		class OriInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			OriInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define ori(rd, rs1, imm) OriInstruction{ imm, rd, rs1 }

		class XoriInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			XoriInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define xori(rd, rs1, imm) XoriInstruction{ imm, rd, rs1 }

		class SlliInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			SlliInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define slli(rd, rs1, imm) SlliInstruction{ imm, rd, rs1 }

		class SrliInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			SrliInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define srli(rd, rs1, imm) SrliInstruction{ imm, rd, rs1 }

		class SraiInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;
		public:
			SraiInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define srai(rd, rs1, imm) SraiInstruction{ imm, rd, rs1 }

		class SltInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			SltInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define slt(rd, rs1, rs2) SltInstruction{ rd, rs1, rs2 }

		class SltuInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			SltuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sltu(rd, rs1, rs2) SltuInstruction{ rd, rs1, rs2 }

		class SltiInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			SltiInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define slti(rd, rs1, imm) SltiInstruction{ imm, rd, rs1 }

		class SltiuInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			SltiuInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sltiu(rd, rs1, imm) SltiuInstruction{ imm, rd, rs1 }

		class JalInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;

		public:
			JalInstruction(native_int imm, RISCVRegister rd)
				: _imm{ imm }, _rd{ rd }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jal(rd, imm) JalInstruction{ imm, rd }

		class JalrInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rd;
			RISCVRegister _rs1;
		public:
			JalrInstruction(native_int imm, RISCVRegister rd, RISCVRegister rs1)
				: _imm{ imm }, _rd{ rd }, _rs1{ rs1 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define jalr(rd, imm, rs1) JalrInstruction{ imm, rd, rs1 }

		class BeqInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			BeqInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define beq(rs1, rs2, imm) BeqInstruction{ imm, rs1, rs2 }

		class BneInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			BneInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define bne(rs1, rs2, imm) BneInstruction{ imm, rs1, rs2 }

		class BltInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			BltInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define blt(rs1, rs2, imm) BltInstruction{ imm, rs1, rs2 }

		class BgeInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			BgeInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define bge(rs1, rs2, imm) BgeInstruction{ imm, rs1, rs2 }

		class BltuInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			BltuInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define bltu(rs1, rs2, imm) BltuInstruction{ imm, rs1, rs2 }

		class BgeuInstruction : public RISCVInstruction
		{
		private:
			native_int _imm;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			BgeuInstruction(native_int imm, RISCVRegister rs1, RISCVRegister rs2)
				: _imm{ imm }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define bgeu(rs1, rs2, imm) BgeuInstruction { imm, rs1, rs2 }

		class RISCVNopInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define nop() RISCVNopInstruction{}

		class RISCVEcallInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define ecall() RISCVEcallInstruction{}

		class RISCVEbreakInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define ebreak() RISCVEbreakInstruction{}

		class RISCVFenceInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define fence() RISCVFenceInstruction{}
	}

	// M extension instructions
	namespace riscv
	{
		class MulInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			MulInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define mul(rd, rs1, rs2) MulInstruction{ rd, rs1, rs2 }

		class MulhInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;

		public:
			MulhInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define mulh(rd, rs1, rs2) MulhInstruction{ rd, rs1, rs2 }

		class MulhuInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			MulhuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define mulhu(rd, rs1, rs2) MulhuInstruction{ rd, rs1, rs2 }

		class MulhsuInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			MulhsuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define mulhsu(rd, rs1, rs2) MulhsuInstruction{ rd, rs1, rs2 }

		class DivInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			DivInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define div(rd, rs1, rs2) DivInstruction{ rd, rs1, rs2 }

		class DivuInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			DivuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define divu(rd, rs1, rs2) DivuInstruction{ rd, rs1, rs2 }

		class RemInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			RemInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define rem(rd, rs1, rs2) RemInstruction{ rd, rs1, rs2 }

		class RemuInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
		public:
			RemuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define remu(rd, rs1, rs2) RemuInstruction{ rd, rs1, rs2 }
	}

	// A extension instructions
	namespace riscv
	{
		class LrwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			bool _acquire;
			bool _release;

		public:
			LrwInstruction(RISCVRegister rd, RISCVRegister rs1, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define lrw(rd, rs1, acquire, release) LrwInstruction{ rd, rs1, acquire, release }

		class ScwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;
		public:
			ScwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define scw(rd, rs2, rs1, acquire, release) ScwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoswapwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmoswapwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amoswapw(rd, rs2, rs1, acquire, release) AmoswapwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoaddwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmoaddwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amoaddw(rd, rs2, rs1, acquire, release) AmoaddwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoandwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmoandwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amoandw(rd, rs2, rs1, acquire, release) AmoandwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoorwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmoorwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amoorw(rd, rs2, rs1, acquire, release) AmoorwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoxorwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmoxorwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amoxorw(rd, rs2, rs1, acquire, release) AmoxorwInstruction{ rd, rs1, rs2, acquire, release }

		class AmominwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmominwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amominw(rd, rs2, rs1, acquire, release) AmominwInstruction{ rd, rs1, rs2, acquire, release }

		class AmomaxwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmomaxwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amomaxw(rd, rs2, rs1, acquire, release) AmomaxwInstruction{ rd, rs1, rs2, acquire, release }

		class AmominuwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmominuwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amominuw(rd, rs2, rs1, acquire, release) AmominuwInstruction{ rd, rs1, rs2, acquire, release }
		class AmomaxuwInstruction : public RISCVInstruction
		{
		private:
			RISCVRegister _rd;
			RISCVRegister _rs1;
			RISCVRegister _rs2;
			bool _acquire;
			bool _release;

		public:
			AmomaxuwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: _rd{ rd }, _rs1{ rs1 }, _rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define amomaxuw(rd, rs2, rs1, acquire, release) AmomaxuwInstruction{ rd, rs1, rs2, acquire, release }
	}

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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
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
			virtual ByteRange emit() const final override;
		};
#define cand(rd, rs2) CandInstruction{ rd, rs2 }

		class CnopInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define cnop() CnopInstruction{}

		class CebreakInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define cebreak() CebreakInstruction{}
	}

	// Zicsr extension instructions
	namespace riscv
	{
		class CsrrwInstruction : public RISCVInstruction
		{
		private:
			native_int _csr;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			CsrrwInstruction(native_int csr, RISCVRegister rd, RISCVRegister rs1)
				: _csr{ csr }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define cssrw(rd, csr, rs1) CssrwInstruction{ csr, rd, rs1 }

		class CsrrsInstruction : public RISCVInstruction
		{
		private:
			native_int _csr;
			RISCVRegister _rd;
			RISCVRegister _rs1;

		public:
			CsrrsInstruction(native_int csr, RISCVRegister rd, RISCVRegister rs1)
				: _csr{ csr }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define csrrs(rd, csr, rs1) CsrrsInstruction{ csr, rd, rs1 }

		class CsrrcInstruction : public RISCVInstruction
		{
		private:
			native_int _csr;
			RISCVRegister _rd;
			RISCVRegister _rs1;
		public:
			CsrrcInstruction(native_int csr, RISCVRegister rd, RISCVRegister rs1)
				: _csr{ csr }, _rd{ rd }, _rs1{ rs1 }
			{
			}

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define csrrc(rd, csr, rs1) CsrrcInstruction{ csr, rd, rs1 }

		class CsrrwiInstruction : public RISCVInstruction
		{
		private:
			native_int _csr;
			RISCVRegister _rd;
			native_int _immediate;
		public:
			CsrrwiInstruction(native_int csr, RISCVRegister rd, native_int immediate)
				: _csr{ csr }, _rd{ rd }, _immediate{ immediate }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define csrrwi(rd, csr, imm) CsrrwiInstruction{ csr, rd, imm }

		class CsrrsiInstruction : public RISCVInstruction
		{
		private:
			native_int _csr;
			RISCVRegister _rd;
			native_int _immediate;
		public:
			CsrrsiInstruction(native_int csr, RISCVRegister rd, native_int immediate)
				: _csr{ csr }, _rd{ rd }, _immediate{ immediate }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define csrrsi(rd, csr, imm) CsrrsiInstruction{ csr, rd, imm }

		class CsrrciInstruction : public RISCVInstruction
		{
		private:
			native_int _csr;
			RISCVRegister _rd;
			native_int _immediate;
		public:
			CsrrciInstruction(native_int csr, RISCVRegister rd, native_int immediate)
				: _csr{ csr }, _rd{ rd }, _immediate{ immediate }
			{
			}
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define csrrci(rd, csr, imm) CsrrciInstruction{ csr, rd, imm }
	}

	// Zifencei extension instructions
	namespace riscv
	{
		class FenceiInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define fencei() FenceiInstruction{}
	}

	// Priv extension instructions
	namespace riscv
	{
		class UretInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define uret() UretInstruction{}

		class SretInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define sret() SretInstruction{}

		class MretInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;
		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define mret() MretInstruction{}

		class RISCVWfiInstruction : public RISCVInstruction
		{
		public:
			using RISCVInstruction::RISCVInstruction;

		public:
			virtual RISCVInstructionType itype() const final override;
			virtual ByteRange emit() const final override;
		};
#define wfi() RISCVWfiInstruction{}
	}
}

#endif
