#ifndef HAZE_RISCVINSTRUCTION_H
#define HAZE_RISCVINSTRUCTION_H

#include <riscv/defs/RISCVInstructionKind.h>
#include <riscv/defs/RISCVRegister.h>
#include <utility/Constants.h>
#include <utility/Variant.h>

// Haze RISCVInstruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	concept RISCVInstructionConcept = requires(const T & t)
	{
		{ t.itype() } -> std::same_as<RISCVInstructionKind>;
		{ t.emit() } -> std::same_as<ByteRange>;
	};

	template<typename T>
	struct RISCVInstructionTrait
	{
		static constexpr bool value = RISCVInstructionConcept<T>;
	};


	// I base instruction set instructions
	namespace riscv
	{
		class LbInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			LbInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lb(rd, imm, rs1) LbInstruction{ imm, rd, rs1 }

		class LhInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			LhInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lh(rd, imm, rs1) LhInstruction{ imm, rd, rs1 }

		class LwInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			LwInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lw(rd, imm, rs1) LwInstruction{ imm, rd, rs1 }

		class LbuInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			LbuInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lbu(rd, imm, rs1) LbuInstruction{ imm, rd, rs1 }

		class LhuInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			LhuInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lhu(rd, imm, rs1) LhuInstruction{ imm, rd, rs1 }
	
		class SbInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs2;
			RISCVRegister rs1;

		public:
			SbInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sb(rs2, imm, rs1) SbInstruction{ imm, rs1, rs2 }

		class ShInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs2;
			RISCVRegister rs1;

		public:
			ShInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sh(rs2, imm, rs1) ShInstruction{ imm, rs1, rs2 }

		class SwInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs2;
			RISCVRegister rs1;

		public:
			SwInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sw(rs2, imm, rs1) SwInstruction{ imm, rs1, rs2 }

		class LuiInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;

		public:
			LuiInstruction(Offset immediate, RISCVRegister rd)
				: immediate{ immediate }, rd{ rd }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lui(rd, imm) LuiInstruction{ imm, rd }

		class AuipcInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;

		public:
			AuipcInstruction(Offset immediate, RISCVRegister rd)
				: immediate{ immediate }, rd{ rd }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define auipc(rd, imm) AuipcInstruction{ imm, rd }

		class AddInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			AddInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define add(rd, rs1, rs2) AddInstruction{ rd, rs1, rs2

		class SubInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			SubInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sub(rd, rs1, rs2) SubInstruction{ rd, rs1, rs2 }

		class AndInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			AndInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define and(rd, rs1, rs2) AndInstruction{ rd, rs1, rs2 }

		class OrInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			OrInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define or(rd, rs1, rs2) OrInstruction{ rd, rs1, rs2 }

		class XorInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			XorInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define xor(rd, rs1, rs2) XorInstruction{ rd, rs1, rs2 }

		class SllInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			SllInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sll(rd, rs1, rs2) SllInstruction{ rd, rs1, rs2 }

		class SrlInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			SrlInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define srl(rd, rs1, rs2) SrlInstruction{ rd, rs1, rs2 }

		class SraInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			SraInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sra(rd, rs1, rs2) SraInstruction{ rd, rs1, rs2 }

		class AddiInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			AddiInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define addi(rd, rs1, imm) AddiInstruction{ imm, rd, rs1 }

		class AndiInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			AndiInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define andi(rd, rs1, imm) AndiInstruction{ imm, rd, rs1 }

		class OriInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			OriInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define ori(rd, rs1, imm) OriInstruction{ imm, rd, rs1 }

		class XoriInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			XoriInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define xori(rd, rs1, imm) XoriInstruction{ imm, rd, rs1 }

		class SlliInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			SlliInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define slli(rd, rs1, imm) SlliInstruction{ imm, rd, rs1 }

		class SrliInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			SrliInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define srli(rd, rs1, imm) SrliInstruction{ imm, rd, rs1 }

		class SraiInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;
		public:
			SraiInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define srai(rd, rs1, imm) SraiInstruction{ imm, rd, rs1 }

		class SltInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			SltInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define slt(rd, rs1, rs2) SltInstruction{ rd, rs1, rs2 }

		class SltuInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			SltuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sltu(rd, rs1, rs2) SltuInstruction{ rd, rs1, rs2 }

		class SltiInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			SltiInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define slti(rd, rs1, imm) SltiInstruction{ imm, rd, rs1 }

		class SltiuInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			SltiuInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sltiu(rd, rs1, imm) SltiuInstruction{ imm, rd, rs1 }

		class JalInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;

		public:
			JalInstruction(Offset immediate, RISCVRegister rd)
				: immediate{ immediate }, rd{ rd }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define jal(rd, imm) JalInstruction{ imm, rd }

		class JalrInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rd;
			RISCVRegister rs1;
		public:
			JalrInstruction(Offset immediate, RISCVRegister rd, RISCVRegister rs1)
				: immediate{ immediate }, rd{ rd }, rs1{ rs1 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define jalr(rd, imm, rs1) JalrInstruction{ imm, rd, rs1 }

		class BeqInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			BeqInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define beq(rs1, rs2, imm) BeqInstruction{ imm, rs1, rs2 }

		class BneInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			BneInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define bne(rs1, rs2, imm) BneInstruction{ imm, rs1, rs2 }

		class BltInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			BltInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define blt(rs1, rs2, imm) BltInstruction{ imm, rs1, rs2 }

		class BgeInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			BgeInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define bge(rs1, rs2, imm) BgeInstruction{ imm, rs1, rs2 }

		class BltuInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			BltuInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define bltu(rs1, rs2, imm) BltuInstruction{ imm, rs1, rs2 }

		class BgeuInstruction
		{
		private:
			Offset immediate;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			BgeuInstruction(Offset immediate, RISCVRegister rs1, RISCVRegister rs2)
				: immediate{ immediate }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define bgeu(rs1, rs2, imm) BgeuInstruction { imm, rs1, rs2 }

		class NopInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define nop() NopInstruction{}

		class EcallInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define ecall() EcallInstruction{}

		class EbreakInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define ebreak() EbreakInstruction{}

		class FenceInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define fence() FenceInstruction{}
	}

	// M extension instructions
	namespace riscv
	{
		class MulInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			MulInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define mul(rd, rs1, rs2) MulInstruction{ rd, rs1, rs2 }

		class MulhInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;

		public:
			MulhInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define mulh(rd, rs1, rs2) MulhInstruction{ rd, rs1, rs2 }

		class MulhuInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			MulhuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define mulhu(rd, rs1, rs2) MulhuInstruction{ rd, rs1, rs2 }

		class MulhsuInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			MulhsuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define mulhsu(rd, rs1, rs2) MulhsuInstruction{ rd, rs1, rs2 }

		class DivInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			DivInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define div(rd, rs1, rs2) DivInstruction{ rd, rs1, rs2 }

		class DivuInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			DivuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define divu(rd, rs1, rs2) DivuInstruction{ rd, rs1, rs2 }

		class RemInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			RemInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define rem(rd, rs1, rs2) RemInstruction{ rd, rs1, rs2 }

		class RemuInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
		public:
			RemuInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define remu(rd, rs1, rs2) RemuInstruction{ rd, rs1, rs2 }
	}

	// A extension instructions
	namespace riscv
	{
		class LrwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			bool _acquire;
			bool _release;

		public:
			LrwInstruction(RISCVRegister rd, RISCVRegister rs1, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define lrw(rd, rs1, acquire, release) LrwInstruction{ rd, rs1, acquire, release }

		class ScwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;
		public:
			ScwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define scw(rd, rs2, rs1, acquire, release) ScwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoswapwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmoswapwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amoswapw(rd, rs2, rs1, acquire, release) AmoswapwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoaddwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmoaddwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amoaddw(rd, rs2, rs1, acquire, release) AmoaddwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoandwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmoandwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amoandw(rd, rs2, rs1, acquire, release) AmoandwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoorwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmoorwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amoorw(rd, rs2, rs1, acquire, release) AmoorwInstruction{ rd, rs1, rs2, acquire, release }

		class AmoxorwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmoxorwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amoxorw(rd, rs2, rs1, acquire, release) AmoxorwInstruction{ rd, rs1, rs2, acquire, release }

		class AmominwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmominwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amominw(rd, rs2, rs1, acquire, release) AmominwInstruction{ rd, rs1, rs2, acquire, release }

		class AmomaxwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmomaxwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amomaxw(rd, rs2, rs1, acquire, release) AmomaxwInstruction{ rd, rs1, rs2, acquire, release }

		class AmominuwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmominuwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amominuw(rd, rs2, rs1, acquire, release) AmominuwInstruction{ rd, rs1, rs2, acquire, release }
		class AmomaxuwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			RISCVRegister rs2;
			bool _acquire;
			bool _release;

		public:
			AmomaxuwInstruction(RISCVRegister rd, RISCVRegister rs1, RISCVRegister rs2, bool acquire, bool release)
				: rd{ rd }, rs1{ rs1 }, rs2{ rs2 }, _acquire{ acquire }, _release{ release }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define amomaxuw(rd, rs2, rs1, acquire, release) AmomaxuwInstruction{ rd, rs1, rs2, acquire, release }
	}

	// C extension instructions
	namespace riscv
	{
		class ClwspInstruction
		{
		private:
			RISCVRegister rd;
			Offset immediate;

		public:
			ClwspInstruction(RISCVRegister rd, Offset immediate)
				: rd{ rd }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define clwsp(rd, imm) ClwspInstruction{ rd, imm }

		class CswspInstruction
		{
		private:
			RISCVRegister rs2;
			Offset immediate;

		public:
			CswspInstruction(RISCVRegister rs2, Offset immediate)
				: rs2{ rs2 }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cswsp(rs2, imm) CswspInstruction{ rs2, imm }

		class ClwInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs1;
			Offset immediate;

		public:
			ClwInstruction(RISCVRegister rd, Offset immediate, RISCVRegister rs1)
				: rd{ rd }, rs1{ rs1 }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define clw(rd, imm, rs1) ClwInstruction{ rd, imm, rs1 }

		class CswInstruction
		{
		private:
			RISCVRegister rs1;
			RISCVRegister rs2;
			Offset immediate;

		public:
			CswInstruction(RISCVRegister rs2, Offset immediate, RISCVRegister rs1)
				: rs1{ rs1 }, rs2{ rs2 }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csw(rs2, imm, rs1) CswInstruction{ rs2, imm, rs1 }

		class CjInstruction
		{
		private:
			Offset immediate;

		public:
			CjInstruction(Offset immediate)
				: immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cj(imm) CjInstruction{ imm }

		class CjalInstruction
		{
		private:
			Offset immediate;

		public:
			CjalInstruction(Offset immediate)
				: immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cjal(imm) CjalInstruction{ imm }

		class CjrInstruction
		{
		private:
			RISCVRegister rs1;

		public:
			CjrInstruction(RISCVRegister rs1)
				: rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cjr(rs1) CjrInstruction{ rs1 }

		class CjalrInstruction
		{
		private:
			RISCVRegister rs1;

		public:
			CjalrInstruction(RISCVRegister rs1)
				: rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cjalr(rs1) CjalrInstruction{ rs1 }

		class CbeqzInstruction
		{
		private:
			RISCVRegister rs2;
			Offset immediate;

		public:
			CbeqzInstruction(RISCVRegister rs2, Offset immediate)
				: rs2{ rs2 }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cbeqz(rs2, imm) CbeqzInstruction{ rs2, imm }

		class CbnezInstruction
		{
		private:
			RISCVRegister rs2;
			Offset immediate;

		public:
			CbnezInstruction(RISCVRegister rs2, Offset immediate)
				: rs2{ rs2 }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cbnez(rs2, imm) CbnezInstruction{ rs2, imm }

		class CliInstruction
		{
		private:
			RISCVRegister rd;
			Offset immediate;

		public:
			CliInstruction(RISCVRegister rd, Offset immediate)
				: rd{ rd }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cli(rd, imm) CliInstruction{ rd, imm }

		class CluiInstruction
		{
		private:
			RISCVRegister rd;
			Offset immediate;

		public:
			CluiInstruction(RISCVRegister rd, Offset immediate)
				: rd{ rd }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define clui(rd, imm) CluiInstruction{ rd, imm }

		class CaddiInstruction
		{
		private:
			RISCVRegister rd;
			Offset immediate;

		public:
			CaddiInstruction(RISCVRegister rd, Offset immediate)
				: rd{ rd }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define caddi(rd, imm) CaddiInstruction{ rd, imm }

		class Caddi16spInstruction
		{
		private:
			Offset immediate;

		public:
			Caddi16spInstruction(Offset immediate)
				: immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define caddi16sp(imm) Caddi16spInstruction{ imm }

		class Caddi4spnInstruction
		{
		private:
			RISCVRegister rd;
			Offset immediate;

		public:
			Caddi4spnInstruction(RISCVRegister rd, Offset immediate)
				: rd{ rd }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define caddi4spn(rd, imm) Caddi4spnInstruction{ rd, imm }

		class CslliInstruction
		{
		private:
			RISCVRegister rd;
			Offset _shamt;

		public:
			CslliInstruction(RISCVRegister rd, Offset shamt)
				: rd{ rd }, _shamt{ shamt }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cslli(rd, shamt) CslliInstruction{ rd, shamt }

		class CsrliInstruction
		{
		private:
			RISCVRegister rd;
			Offset _shamt;

		public:
			CsrliInstruction(RISCVRegister rd, Offset shamt)
				: rd{ rd }, _shamt{ shamt }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrli(rd, shamt) CsrliInstruction{ rd, shamt }

		class CsraiInstruction
		{
		private:
			RISCVRegister rd;
			Offset _shamt;

		public:
			CsraiInstruction(RISCVRegister rd, Offset shamt)
				: rd{ rd }, _shamt{ shamt }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrai(rd, shamt) CsraiInstruction{ rd, shamt }

		class CandiInstruction
		{
		private:
			RISCVRegister rd;
			Offset immediate;

		public:
			CandiInstruction(RISCVRegister rd, Offset immediate)
				: rd{ rd }, immediate{ immediate }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define candi(rd, imm) CandiInstruction{ rd, imm }

		class CmvInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs2;

		public:
			CmvInstruction(RISCVRegister rd, RISCVRegister rs2)
				: rd{ rd }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cmv(rd, rs2) CmvInstruction{ rd, rs2 }

		class CaddInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs2;

		public:
			CaddInstruction(RISCVRegister rd, RISCVRegister rs2)
				: rd{ rd }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cadd(rd, rs2) CaddInstruction{ rd, rs2 }

		class CsubInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs2;

		public:
			CsubInstruction(RISCVRegister rd, RISCVRegister rs2)
				: rd{ rd }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csub(rd, rs2) CsubInstruction{ rd, rs2 }

		class CxorInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs2;

		public:
			CxorInstruction(RISCVRegister rd, RISCVRegister rs2)
				: rd{ rd }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cxor(rd, rs2) CxorInstruction{ rd, rs2 }

		class CorInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs2;

		public:
			CorInstruction(RISCVRegister rd, RISCVRegister rs2)
				: rd{ rd }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cor(rd, rs2) CorInstruction{ rd, rs2 }

		class CandInstruction
		{
		private:
			RISCVRegister rd;
			RISCVRegister rs2;

		public:
			CandInstruction(RISCVRegister rd, RISCVRegister rs2)
				: rd{ rd }, rs2{ rs2 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cand(rd, rs2) CandInstruction{ rd, rs2 }

		class CnopInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cnop() CnopInstruction{}

		class CebreakInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cebreak() CebreakInstruction{}
	}

	// Zicsr extension instructions
	namespace riscv
	{
		class CsrrwInstruction
		{
		private:
			Offset csr;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			CsrrwInstruction(Offset csr, RISCVRegister rd, RISCVRegister rs1)
				: csr{ csr }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cssrw(rd, csr, rs1) CssrwInstruction{ csr, rd, rs1 }

		class CsrrsInstruction
		{
		private:
			Offset csr;
			RISCVRegister rd;
			RISCVRegister rs1;

		public:
			CsrrsInstruction(Offset csr, RISCVRegister rd, RISCVRegister rs1)
				: csr{ csr }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrrs(rd, csr, rs1) CsrrsInstruction{ csr, rd, rs1 }

		class CsrrcInstruction
		{
		private:
			Offset csr;
			RISCVRegister rd;
			RISCVRegister rs1;
		public:
			CsrrcInstruction(Offset csr, RISCVRegister rd, RISCVRegister rs1)
				: csr{ csr }, rd{ rd }, rs1{ rs1 }
			{
			}

		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrrc(rd, csr, rs1) CsrrcInstruction{ csr, rd, rs1 }

		class CsrrwiInstruction
		{
		private:
			Offset csr;
			RISCVRegister rd;
			Offset immediate;
		public:
			CsrrwiInstruction(Offset csr, RISCVRegister rd, Offset immediate)
				: csr{ csr }, rd{ rd }, immediate{ immediate }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrrwi(rd, csr, imm) CsrrwiInstruction{ csr, rd, imm }

		class CsrrsiInstruction
		{
		private:
			Offset csr;
			RISCVRegister rd;
			Offset immediate;
		public:
			CsrrsiInstruction(Offset csr, RISCVRegister rd, Offset immediate)
				: csr{ csr }, rd{ rd }, immediate{ immediate }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrrsi(rd, csr, imm) CsrrsiInstruction{ csr, rd, imm }

		class CsrrciInstruction
		{
		private:
			Offset csr;
			RISCVRegister rd;
			Offset immediate;
		public:
			CsrrciInstruction(Offset csr, RISCVRegister rd, Offset immediate)
				: csr{ csr }, rd{ rd }, immediate{ immediate }
			{
			}
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define csrrci(rd, csr, imm) CsrrciInstruction{ csr, rd, imm }
	}

	// Zifencei extension instructions
	namespace riscv
	{
		class FenceiInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define fencei() FenceiInstruction{}
	}

	// Priv extension instructions
	namespace riscv
	{
		class UretInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define uret() UretInstruction{}

		class SretInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sret() SretInstruction{}

		class MretInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define mret() MretInstruction{}

		class WfiInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define wfi() WfiInstruction{}
	}

	using RISCVInstruction = ConstrainedVariant<RISCVInstructionTrait,
		// I base instructions
		riscv::LbInstruction,
		riscv::LhInstruction,
		riscv::LwInstruction,
		riscv::LbuInstruction,
		riscv::LhuInstruction,
		riscv::SbInstruction,
		riscv::ShInstruction,
		riscv::SwInstruction,
		// riscv::LiInstruction, // pseudo-instruction implemented on the assembler side
		riscv::LuiInstruction,
		riscv::AuipcInstruction,
		riscv::AddiInstruction,
		riscv::AddInstruction,
		riscv::SubInstruction,
		riscv::SltiInstruction,
		riscv::SltInstruction,
		riscv::SltiuInstruction,
		riscv::SltuInstruction,
		riscv::AndiInstruction,
		riscv::AndInstruction,
		riscv::OriInstruction,
		riscv::OrInstruction,
		riscv::XoriInstruction,
		riscv::XorInstruction,
		riscv::SlliInstruction,
		riscv::SllInstruction,
		riscv::SrliInstruction,
		riscv::SrlInstruction,
		riscv::SraiInstruction,
		riscv::SraInstruction,
		riscv::JalInstruction,
		riscv::JalrInstruction,
		riscv::BeqInstruction,
		riscv::BneInstruction,
		riscv::BltInstruction,
		riscv::BgeInstruction,
		riscv::BltuInstruction,
		riscv::BgeuInstruction,
		riscv::NopInstruction,
		riscv::EcallInstruction,
		riscv::EbreakInstruction,
		riscv::FenceInstruction,
		
		// M extension
		riscv::MulInstruction,
		riscv::MulhInstruction,
		riscv::MulhuInstruction,
		riscv::MulhsuInstruction,
		riscv::DivInstruction,
		riscv::DivuInstruction,
		riscv::RemInstruction,
		riscv::RemuInstruction,

		// A extension
		riscv::LrwInstruction,
		riscv::ScwInstruction,
		riscv::AmoswapwInstruction,
		riscv::AmoaddwInstruction,
		riscv::AmoxorwInstruction,
		riscv::AmoandwInstruction,
		riscv::AmoorwInstruction,
		riscv::AmominwInstruction,
		riscv::AmomaxwInstruction,
		riscv::AmominuwInstruction,
		riscv::AmomaxuwInstruction,

		// C extension
		riscv::ClwspInstruction,
		riscv::CswspInstruction,
		riscv::ClwInstruction,
		riscv::CswInstruction,
		riscv::CjInstruction,
		riscv::CjalInstruction,
		riscv::CjrInstruction,
		riscv::CjalrInstruction,
		riscv::CbeqzInstruction,
		riscv::CbnezInstruction,
		riscv::CliInstruction,
		riscv::CluiInstruction,
		riscv::CaddiInstruction,
		riscv::Caddi16spInstruction,
		riscv::Caddi4spnInstruction,
		riscv::CslliInstruction,
		riscv::CsrliInstruction,
		riscv::CsraiInstruction,
		riscv::CandiInstruction,
		riscv::CmvInstruction,
		riscv::CaddInstruction,
		riscv::CorInstruction,
		riscv::CxorInstruction,
		riscv::CsubInstruction,
		riscv::CnopInstruction,
		riscv::CebreakInstruction,

		// Zicsr extension
		riscv::CsrrwInstruction,
		riscv::CsrrsInstruction,
		riscv::CsrrcInstruction,
		riscv::CsrrwiInstruction,
		riscv::CsrrsiInstruction,
		riscv::CsrrciInstruction,

		// Zifencei extension
		riscv::FenceiInstruction,

		// Priv extension
		riscv::UretInstruction,
		riscv::SretInstruction,
		riscv::MretInstruction,
		riscv::WfiInstruction
	>;
}

#endif
