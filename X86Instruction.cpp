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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(_destination->otype()));
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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("movzx", _operand_type_map.at(_destination->otype()));
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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("add", _operand_type_map.at(_destination->otype()));
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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("sub", _operand_type_map.at(_destination->otype()));
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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("or", _operand_type_map.at(_destination->otype()));
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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("xor", _operand_type_map.at(_destination->otype()));
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
					} break;
				}
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("test", _operand_type_map.at(_destination->otype()));
			} break;
		}
	}
}
