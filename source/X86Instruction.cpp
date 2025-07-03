import std;

#include "X86Instruction.h"
#include "X86Builder.h"
#include "CommonErrors.h"

// Haze X86Instruction.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	X86InstructionType X86PushInstruction::itype() const
	{
		return X86InstructionType::PUSH;
	}

	byterange X86PushInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case IMMEDIATE:
			{
				// TODO: push imm8 when the operand is small enough
				auto immediate_operand = AS_IMMEDIATE_OPERAND(_operand);
				return X86Builder::push_i32(immediate_operand->_immediate);
			} break;

			case INDIRECT:
			{
				auto indirect_operand = AS_INDIRECT_OPERAND(_operand);
				return X86Builder::push_m(indirect_operand->_address);
			} break;

			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::push_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("push", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86PopInstruction::itype() const
	{
		return X86InstructionType::POP;
	}

	byterange X86PopInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::pop_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("pop", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86MovInstruction::itype() const
	{
		return X86InstructionType::MOV;
	}

	byterange X86MovInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case INDIRECT:
			{
				auto indirect_destination = AS_INDIRECT_OPERAND(_destination);

				switch (_source->otype())
				{
					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::mov_mr(indirect_destination->_address, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::mov_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case INDIRECT:
					{
						auto indirect_source = AS_INDIRECT_OPERAND(_source);
						return X86Builder::mov_rm(register_destination->_register, indirect_source->_address);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::mov_rr(register_destination->_register, register_source->_register);
					} break;

					case REGISTER_DISPLACED:
					{
						auto register_displaced_source = AS_REGISTER_DISPLACED_OPERAND(_source);
						return X86Builder::mov_rbd(register_destination->_register, register_displaced_source->_register, register_displaced_source->_displacement);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			case REGISTER_DISPLACED:
			{
				auto register_displaced_destination = AS_REGISTER_DISPLACED_OPERAND(_destination);
				
				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::mov_bdi(register_displaced_destination->_register, register_displaced_destination->_displacement, immediate_source->_immediate);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86MovzxInstruction::itype() const
	{
		return X86InstructionType::MOVZX;
	}

	byterange X86MovzxInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::movzx_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("movzx", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("movzx", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86AddInstruction::itype() const
	{
		return X86InstructionType::ADD;
	}

	byterange X86AddInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::add_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::add_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("add", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("add", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86SubInstruction::itype() const
	{
		return X86InstructionType::SUB;
	}

	byterange X86SubInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::sub_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::sub_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("sub", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("sub", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86OrInstruction::itype() const
	{
		return X86InstructionType::OR;
	}

	byterange X86OrInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::or_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::or_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("or", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("or", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86AndInstruction::itype() const
	{
		return X86InstructionType::AND;
	}

	byterange X86AndInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::and_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::and_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("and", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("and", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86XorInstruction::itype() const
	{
		return X86InstructionType::XOR;
	}

	byterange X86XorInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::xor_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::xor_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("xor", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("xor", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86IncInstruction::itype() const
	{
		return X86InstructionType::INC;
	}

	byterange X86IncInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::inc_r(register_operand->_register);
			} break;
			
			default:
			{
				CommonErrors::unsupported_instruction_format("inc", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86DecInstruction::itype() const
	{
		return X86InstructionType::DEC;
	}

	byterange X86DecInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::dec_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("dec", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86TestInstruction::itype() const
	{
		return X86InstructionType::TEST;
	}

	byterange X86TestInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::test_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("test", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("test", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86CmpInstruction::itype() const
	{
		return X86InstructionType::CMP;
	}

	byterange X86CmpInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						return X86Builder::cmp_ri(register_destination->_register, immediate_source->_immediate);
					} break;

					case REGISTER:
					{
						auto register_source = AS_REGISTER_OPERAND(_source);
						return X86Builder::cmp_rr(register_destination->_register, register_source->_register);
					} break;

					default:
					{
						CommonErrors::unsupported_instruction_format("cmp", _operand_type_map.at(_source->otype()));
						return {};
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("cmp", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType X86JccInstruction::itype() const
	{
		return X86InstructionType::JCC;
	}

	// NOTE: no implementation for instruction derived-base classes
	/*byterange X86JccInstruction::emit() const
	{
	}*/


	X86JccInstructionType X86ApiCallInstruction::jtype() const
	{
		return X86JccInstructionType::API_CALL;
	}

	byterange X86ApiCallInstruction::emit() const
	{
		return X86Builder::call_absolute(_displacement);
	}


	X86JccInstructionType X86CallInstruction::jtype() const
	{
		return X86JccInstructionType::CALL;
	}

	byterange X86CallInstruction::emit() const
	{
		return X86Builder::call_relative(_displacement);
	}


	X86JccInstructionType X86JmpInstruction::jtype() const
	{
		return X86JccInstructionType::JMP;
	}

	byterange X86JmpInstruction::emit() const
	{
		return X86Builder::jmp_relative(_displacement);
	}


	X86JccInstructionType X86JeInstruction::jtype() const
	{
		return X86JccInstructionType::JE;
	}

	byterange X86JeInstruction::emit() const
	{
		return X86Builder::je_relative(_displacement);
	}


	X86JccInstructionType X86JneInstruction::jtype() const
	{
		return X86JccInstructionType::JNE;
	}

	byterange X86JneInstruction::emit() const
	{
		return X86Builder::jne_relative(_displacement);
	}


	X86JccInstructionType X86JlInstruction::jtype() const
	{
		return X86JccInstructionType::JL;
	}

	byterange X86JlInstruction::emit() const
	{
		return X86Builder::jl_relative(_displacement);
	}


	X86JccInstructionType X86JleInstruction::jtype() const
	{
		return X86JccInstructionType::JLE;
	}

	byterange X86JleInstruction::emit() const
	{
		return X86Builder::jle_relative(_displacement);
	}


	X86JccInstructionType X86JgInstruction::jtype() const
	{
		return X86JccInstructionType::JG;
	}

	byterange X86JgInstruction::emit() const
	{
		return X86Builder::jg_relative(_displacement);
	}


	X86JccInstructionType X86JgeInstruction::jtype() const
	{
		return X86JccInstructionType::JGE;
	}

	byterange X86JgeInstruction::emit() const
	{
		return X86Builder::jge_relative(_displacement);
	}


	X86JccInstructionType X86JaInstruction::jtype() const
	{
		return X86JccInstructionType::JA;
	}

	byterange X86JaInstruction::emit() const
	{
		return X86Builder::ja_relative(_displacement);
	}


	X86JccInstructionType X86JaeInstruction::jtype() const
	{
		return X86JccInstructionType::JAE;
	}

	byterange X86JaeInstruction::emit() const
	{
		return X86Builder::jae_relative(_displacement);
	}


	X86JccInstructionType X86JbInstruction::jtype() const
	{
		return X86JccInstructionType::JB;
	}

	byterange X86JbInstruction::emit() const
	{
		return X86Builder::jb_relative(_displacement);
	}


	X86JccInstructionType X86JbeInstruction::jtype() const
	{
		return X86JccInstructionType::JBE;
	}

	byterange X86JbeInstruction::emit() const
	{
		return X86Builder::jbe_relative(_displacement);
	}


	X86InstructionType X86SetccInstruction::itype() const
	{
		return X86InstructionType::SETCC;
	}

	// NOTE: no implementation for instruction derived-base classes
	/*byterange X86SetccInstruction::emit() const
	{
	}*/

	
	X86SetccInstructionType X86SeteInstruction::stype() const
	{
		return X86SetccInstructionType::SETE;
	}

	byterange X86SeteInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::sete_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("sete", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetneInstruction::stype() const
	{
		return X86SetccInstructionType::SETNE;
	}

	byterange X86SetneInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setne_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setne", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetlInstruction::stype() const
	{
		return X86SetccInstructionType::SETL;
	}

	byterange X86SetlInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setl_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setl", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetleInstruction::stype() const
	{
		return X86SetccInstructionType::SETLE;
	}

	byterange X86SetleInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setle_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setle", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetgInstruction::stype() const
	{
		return X86SetccInstructionType::SETG;
	}

	byterange X86SetgInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setg_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setg", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetgeInstruction::stype() const
	{
		return X86SetccInstructionType::SETGE;
	}

	byterange X86SetgeInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setge_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setge", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetaInstruction::stype() const
	{
		return X86SetccInstructionType::SETA;
	}

	byterange X86SetaInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::seta_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("seta", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetaeInstruction::stype() const
	{
		return X86SetccInstructionType::SETAE;
	}

	byterange X86SetaeInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setae_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setae", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetbInstruction::stype() const
	{
		return X86SetccInstructionType::SETB;
	}

	byterange X86SetbInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setb_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setb", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86SetccInstructionType X86SetbeInstruction::stype() const
	{
		return X86SetccInstructionType::SETBE;
	}

	byterange X86SetbeInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				auto register_operand = AS_REGISTER_OPERAND(_operand);
				return X86Builder::setbe_r(register_operand->_register);
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setbe", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}
}
