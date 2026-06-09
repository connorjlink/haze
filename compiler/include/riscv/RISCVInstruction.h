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
	concept IsRISCVInstruction = requires(const T & t)
	{
		{ t.itype() } -> std::same_as<RISCVInstructionKind>;
		{ t.emit() } -> std::same_as<ByteRange>;
	};

	template<typename T>
	struct RISCVInstructionTrait
	{
		static constexpr bool value = IsRISCVInstruction<T>;
	};


	// I base instruction set instructions
	namespace riscv
	{
		struct LbInstruction
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

		struct LhInstruction
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

		struct LwInstruction
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

		struct LbuInstruction
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

		struct LhuInstruction
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
	
		struct SbInstruction
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

		struct ShInstruction
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

		struct SwInstruction
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

		struct LuiInstruction
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

		struct AuipcInstruction
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

		struct AddInstruction
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

		struct SubInstruction
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

		struct AndInstruction
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

		struct OrInstruction
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

		struct XorInstruction
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

		struct SllInstruction
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

		struct SrlInstruction
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

		struct SraInstruction
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

		struct AddiInstruction
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

		struct AndiInstruction
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

		struct OriInstruction
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

		struct XoriInstruction
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

		struct SlliInstruction
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

		struct SrliInstruction
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

		struct SraiInstruction
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

		struct SltInstruction
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

		struct SltuInstruction
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

		struct SltiInstruction
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

		struct SltiuInstruction
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

		struct JalInstruction
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

		struct JalrInstruction
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

		struct BeqInstruction
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

		struct BneInstruction
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

		struct BltInstruction
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

		struct BgeInstruction
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

		struct BltuInstruction
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

		struct BgeuInstruction
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

		struct NopInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define nop() NopInstruction{}

		struct EcallInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define ecall() EcallInstruction{}

		struct EbreakInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define ebreak() EbreakInstruction{}

		struct FenceInstruction
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
		struct MulInstruction
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

		struct MulhInstruction
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

		struct MulhuInstruction
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

		struct MulhsuInstruction
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

		struct DivInstruction
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

		struct DivuInstruction
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

		struct RemInstruction
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

		struct RemuInstruction
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
		struct LrwInstruction
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

		struct ScwInstruction
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

		struct AmoswapwInstruction
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

		struct AmoaddwInstruction
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

		struct AmoandwInstruction
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

		struct AmoorwInstruction
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

		struct AmoxorwInstruction
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

		struct AmominwInstruction
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

		struct AmomaxwInstruction
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

		struct AmominuwInstruction
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
		struct AmomaxuwInstruction
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
		struct ClwspInstruction
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

		struct CswspInstruction
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

		struct ClwInstruction
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

		struct CswInstruction
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

		struct CjInstruction
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

		struct CjalInstruction
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

		struct CjrInstruction
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

		struct CjalrInstruction
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

		struct CbeqzInstruction
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

		struct CbnezInstruction
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

		struct CliInstruction
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

		struct CluiInstruction
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

		struct CaddiInstruction
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

		struct Caddi16spInstruction
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

		struct Caddi4spnInstruction
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

		struct CslliInstruction
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

		struct CsrliInstruction
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

		struct CsraiInstruction
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

		struct CandiInstruction
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

		struct CmvInstruction
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

		struct CaddInstruction
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

		struct CsubInstruction
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

		struct CxorInstruction
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

		struct CorInstruction
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

		struct CandInstruction
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

		struct CnopInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define cnop() CnopInstruction{}

		struct CebreakInstruction
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
		struct CsrrwInstruction
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

		struct CsrrsInstruction
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

		struct CsrrcInstruction
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

		struct CsrrwiInstruction
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

		struct CsrrsiInstruction
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

		struct CsrrciInstruction
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
		struct FenceiInstruction
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
		struct UretInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define uret() UretInstruction{}

		struct SretInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define sret() SretInstruction{}

		struct MretInstruction
		{
		public:
			RISCVInstructionKind itype() const;
			ByteRange emit() const;
		};
#define mret() MretInstruction{}

		struct WfiInstruction
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
