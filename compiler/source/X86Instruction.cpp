import std;

#include <error/CommonErrors.h>
#include <x86/X86Instruction.h>
#include <x86/X86Builder.h>
#include <utility/BinaryUtilities.h>

// Haze X86Instruction.cpp
// (c) Connor J. Link. All Rights Reserved.

#define VERIFY_REGISTER(x) ([&] -> decltype(x) { \
	if (x & ~0b111) \
	{ \
		CommonErrors::invalid_register(std::to_string(x), NULL_TOKEN); \
		return {}; \
	} \
	return x; \
	}())

namespace
{
	static constexpr std::uint8_t OPERAND_SIZE_OVERRIDE_PREFIX = 0x66;
	static constexpr std::uint8_t ADDRESS_SIZE_OVERRIDE_PREFIX = 0x67;
}

namespace hz::x86
{
	X86OperandType ImmediateOperand::otype() const
	{
		return X86OperandType::IMMEDIATE;
	}

	X86OperandType IndirectOperand::otype() const
	{
		return X86OperandType::INDIRECT;
	}

	X86OperandType RegisterOperand::otype() const
	{
		return X86OperandType::REGISTER;
	}

	X86OperandType RegisterIndirectOperand::otype() const
	{
		return X86OperandType::REGISTER_INDIRECT;
	}

	X86OperandType RegisterDisplacedOperand::otype() const
	{
		return X86OperandType::REGISTER_DISPLACED;
	}


	X86InstructionType PushInstruction::itype() const
	{
		return X86InstructionType::PUSH;
	}

	ByteRange PushInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case IMMEDIATE:
			{
				const auto immediate_operand = AS_IMMEDIATE_OPERAND(operand.get());
				const auto immediate = immediate_operand->immediate;

				// NOTE: 8- and 16-bits are implicitly sign-extended to 32-bits when executed
				if (immediate.is_within_range<std::int8_t>())
				{
					ByteRange out{};

					// 6A --> PUSH imm8
					PUT(range8(0x6A));
					PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

					return out;
				} 
				else if (immediate.is_within_range<std::int32_t>())
				{
					ByteRange out{};

					// 6A --> PUSH imm8
					PUT(range8(0x6A));
					PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

					return out;
				}

				CommonErrors::unsupported_instruction_range("push", immediate.to_string());
				return {};
			} break;

			// dereferencing pointer
			case INDIRECT:
			{
				const auto indirect_operand = AS_INDIRECT_OPERAND(operand.get());
				const auto address = indirect_operand->_address;

				ByteRange out{};

				// FF /6 --> PUSH r/m32
				PUT(range8(0xFF));
				PUT(range8(0x35));
				PUT(range32(address));

				return out;
			} break;

			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto source = VERIFY_REGISTER(register_operand->_register);

				// 50+rd --> PUSH r32
				return { static_cast<std::uint8_t>(0x50 | source) };
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("push", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType PopInstruction::itype() const
	{
		return X86InstructionType::POP;
	}

	ByteRange PopInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				// 58+ rd --> POP r32
				return { static_cast<std::uint8_t>(0x58 | destination) };
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("pop", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType MovInstruction::itype() const
	{
		return X86InstructionType::MOV;
	}

	ByteRange MovInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			// dereferencing pointer
			case INDIRECT:
			{
				const auto indirect_destination = AS_INDIRECT_OPERAND(destination.get());

				switch (_source->otype())
				{
					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = indirect_destination->_address;
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 89 /r --> MOV r/m32, r32
						PUT(range8(0x89));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// B8+rd id --> MOV r32, imm32
							PUT(range8(0xB8 | destination));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("mov", immediate.to_string());
						return {};
					} break;

					// dereferencing pointer
					case INDIRECT:
					{
						const auto indirect_source = AS_INDIRECT_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = indirect_source->_address;

						ByteRange out{};

						if (destination == EAX)
						{
							// A1 --> MOV EAX, moffs32*
							PUT(range8(0xA1));
							PUT(range32(source));
						}
						else
						{
							// 8B /r --> MOV r32, r/m32
							PUT(range8(0x8B));
							PUT(range8(X86Builder::modrm(0b00, destination, 0b101)));
							PUT(range32(source));
						}

						return out;
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 89 /r --> MOV r/m32, r32
						PUT(range8(0x89));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
					} break;

					case REGISTER_DISPLACED:
					{
						const auto register_displaced_source = AS_REGISTER_DISPLACED_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_displaced_source->_register);
						const auto displacement = register_displaced_source->_displacement;

						ByteRange out{};

						if (EI(static_cast<std::intmax_t>(displacement)).is_within_range<std::int8_t>())
						{
							// 8B /r --> MOV r32, r/m32
							PUT(range8(0x8B));
							PUT(range8(X86Builder::modrm(0b01, destination, source)));
							PUT(range32(std::bit_cast<std::uint32_t>(displacement)));
						}
						else
						{
							// 8B /r --> MOV r32, r/m32
							PUT(range8(0x8B));
							PUT(range8(X86Builder::modrm(0b10, destination, source)));
							PUT(range32(std::bit_cast<std::uint32_t>(displacement)));
						}
						
						return out;
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
				const auto register_displaced_destination = AS_REGISTER_DISPLACED_OPERAND(destination.get());
				
				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_displaced_destination->_register);
						const auto displacement = register_displaced_destination->_displacement;
						const auto immediate = immediate_source->immediate;

						if (!EI(static_cast<std::intmax_t>(displacement)).is_within_range<std::int8_t>())
						{
							CommonErrors::unsupported_instruction_range("mov", std::to_string(displacement));
							return {};
						}


						if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// C7 /0 --> MOV r/m32, imm32
							PUT(range8(0xC7));
							PUT(range8(X86Builder::modrm(0b01, 0b000, destination)));
							PUT(range8(displacement));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("mov", immediate.to_string());
						return {};
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
				CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType MovzxInstruction::itype() const
	{
		return X86InstructionType::MOVZX;
	}

	ByteRange MovzxInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 0F B6 /r --> MOVZX r32, r/m8
						PUT(range8(0x0F));
						PUT(range8(0xB6));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("movzx", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType AddInstruction::itype() const
	{
		return X86InstructionType::ADD;
	}

	ByteRange AddInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								ByteRange out{};

								// 05 id --> ADD EAX, imm32
								PUT(range8(0x05));
								PUT(range32(value));

								return out;
							}
							else
							{
								CommonErrors::unsupported_instruction_range("add", immediate.to_string());
								return {};
							}
						}

						if (immediate.is_within_range<std::int8_t>())
						{
							ByteRange out{};
							
							// 83 /0 ib --> ADD r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b000, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));
							
							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};
							
							// 81 /0 id --> ADD r/m32, imm32
							PUT(range8(0x81));
							PUT(range8(X86Builder::modrm(0b11, 0b000, destination)));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));
							
							return out;
						}

						CommonErrors::unsupported_instruction_range("add", immediate.to_string());
						return {};
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 01 /r --> ADD r/m32, r32
						PUT(range8(0x01));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;

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
				CommonErrors::unsupported_instruction_format("add", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SubInstruction::itype() const
	{
		return X86InstructionType::SUB;
	}

	ByteRange SubInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								ByteRange out{};

								// 2D id --> SUB EAX, imm32
								PUT(range8(0x2D));
								PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

								return out;
							}
							else
							{
								CommonErrors::unsupported_instruction_range("sub", immediate.to_string());
								return {};
							}
						}

						if (immediate.is_within_range<std::int8_t>())
						{
							ByteRange out{};

							// 83 /5 ib --> SUB r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b101, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// 81 /5 id --> SUB r/m32, imm32
							PUT(range8(0x81));
							PUT(range8(X86Builder::modrm(0b11, 0b101, destination)));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("sub", immediate.to_string());
						return {};
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 29 /r --> SUB r/m32, r32
						PUT(range8(0x29));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("sub", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType OrInstruction::itype() const
	{
		return X86InstructionType::OR;
	}

	ByteRange OrInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								ByteRange out{};

								// 0D id --> OR EAX, imm32
								PUT(range8(0x0D));
								PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

								return out;
							}
							else
							{
								CommonErrors::unsupported_instruction_range("or", immediate.to_string());
								return {};
							}
						}

						if (immediate.is_within_range<std::int8_t>())
						{
							ByteRange out{};

							// 83 /1 ib --> OR r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b001, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// 81 /1 id --> OR r/m32, imm32
							PUT(range8(0x81));
							PUT(range8(X86Builder::modrm(0b11, 0b001, destination)));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("or", immediate.to_string());
						return {};
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 09 /r --> OR r/m32, r32
						PUT(range8(0x09));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("or", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType AndInstruction::itype() const
	{
		return X86InstructionType::AND;
	}

	ByteRange AndInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								ByteRange out{};

								// 25 id --> AND EAX, imm32
								PUT(range8(0x25));
								PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

								return out;
							}
							else
							{
								CommonErrors::unsupported_instruction_range("and", immediate.to_string());
								return {};
							}
						}
						
						if (immediate.is_within_range<std::int8_t>())
						{
							ByteRange out{};

							// 83 /4 ib --> AND r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// 81 /4 id --> AND r/m32, imm32
							PUT(range8(0x81));
							PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("and", immediate.to_string());
						return {};
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 21 /r --> AND r/m32, r32
						PUT(range8(0x21));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("and", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType XorInstruction::itype() const
	{
		return X86InstructionType::XOR;
	}

	ByteRange XorInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								ByteRange out{};

								// 35 id --> XOR EAX, imm32
								PUT(range8(0x35));
								PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

								return out;
							}
							else
							{
								CommonErrors::unsupported_instruction_range("xor", immediate.to_string());
								return {};
							}
						}

						if (immediate.is_within_range<std::int8_t>())
						{
							ByteRange out{};

							// 83 /6 ib --> XOR r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b110, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// 81 /6 id --> XOR r/m32, imm32
							PUT(range8(0x81));
							PUT(range8(X86Builder::modrm(0b11, 0b110, destination)));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("xor", immediate.to_string());
						return {};
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// XOR --> r/m32, r32
						PUT(range8(0x31));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("xor", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType IncInstruction::itype() const
	{
		return X86InstructionType::INC;
	}

	ByteRange IncInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto source = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 40+rd --> INC r32
				PUT(range8(0x40 | source));

				return out;
			} break;
			
			default:
			{
				CommonErrors::unsupported_instruction_format("inc", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType DecInstruction::itype() const
	{
		return X86InstructionType::DEC;
	}

	ByteRange DecInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto source = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 48+rd --> DEC r32
				PUT(range8(0x48 | source));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("dec", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SalInstruction::itype() const
	{
		return X86InstructionType::SAL;
	}

	ByteRange SalInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_destination->_register);

				const auto ei = EI(static_cast<std::intmax_t>(immediate));

				if (ei == EI(std::intmax_t{ 1 }))
				{
					ByteRange out{};

					// D1 /4 --> SAL r/m32, 1
					PUT(range8(0xD1));
					PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));

					return out;
				}
				else if (ei.is_within_range<std::int8_t>())
				{
					ByteRange out{};

					// C1 /4 ib --> SAL r/m32, imm8
					PUT(range8(0xC1));
					PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));
					PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>()));

					return out;
				}

				CommonErrors::unsupported_instruction_range("sal", ei.to_string());
				return {};
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("sal", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SarInstruction::itype() const
	{
		return X86InstructionType::SAR;
	}

	ByteRange SarInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
		case REGISTER:
		{
			const auto register_destination = AS_REGISTER_OPERAND(operand.get());
			const auto destination = VERIFY_REGISTER(register_destination->_register);

			const auto ei = EI(static_cast<std::intmax_t>(immediate));

			if (ei == EI(std::intmax_t{ 1 }))
			{
				ByteRange out{};

				// D1 /7 --> SAR r/m32, 1
				PUT(range8(0xD1));
				PUT(range8(X86Builder::modrm(0b11, 0b111, destination)));

				return out;
			}
			else if (ei.is_within_range<std::int8_t>())
			{
				ByteRange out{};

				// C1 /7 ib --> SAR r/m32, imm8
				PUT(range8(0xC1));
				PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));
				PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>()));

				return out;
			}

			CommonErrors::unsupported_instruction_range("sar", ei.to_string());
			return {};
		} break;

		default:
		{
			CommonErrors::unsupported_instruction_format("sar", _operand_type_map.at(operand->otype()));
			return {};
		} break;
		}
	}


	X86InstructionType TestInstruction::itype() const
	{
		return X86InstructionType::TEST;
	}

	ByteRange TestInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 85 /r --> TEST r/m32, r32
						PUT(range8(0x85));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("test", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType CmpInstruction::itype() const
	{
		return X86InstructionType::CMP;
	}

	ByteRange CmpInstruction::emit() const
	{
		using enum X86OperandType;
		switch (destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(destination.get());

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								ByteRange out{};

								// 3D id --> CMP EAX, imm32
								PUT(range8(0x3D));
								PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

								return out;
							}
							else
							{
								CommonErrors::unsupported_instruction_range("cmp", immediate.to_string());
								return {};
							}
						}

						if (immediate.is_within_range<std::int8_t>())
						{
							ByteRange out{};

							// 83 /7 ib --> CMP r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b111, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							ByteRange out{};

							// 81 /7 id --> CMP r/m32, imm32
							PUT(range8(0x81));
							PUT(range8(X86Builder::modrm(0b11, 0b111, destination)));
							PUT(range32(std::bit_cast<std::uint32_t>(immediate.to_integral<std::int32_t>())));

							return out;
						}

						CommonErrors::unsupported_instruction_range("cmp", immediate.to_string());
						return {};
					} break;

					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source.get());
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						ByteRange out{};

						// 39 /r --> CMP r/m32, r32
						PUT(range8(0x39));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
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
				CommonErrors::unsupported_instruction_format("cmp", _operand_type_map.at(destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType CallInstruction::itype() const
	{
		return X86InstructionType::CALL;
	}

	ByteRange CallInstruction::emit() const
	{
		ByteRange out{};

		// E8 cd --> CALL rel32
		PUT(range8(0xE8));
		PUT(range32(std::bit_cast<std::uint32_t>(displacement)));

		return out;
	}


	X86InstructionType ApicallInstruction::itype() const
	{
		return X86InstructionType::APICALL;
	}

	ByteRange ApicallInstruction::emit() const
	{
		ByteRange out{};

		// FF /2 --> CALL r/m32
		PUT(range8(0xFF));
		PUT(range8(0x15));
		PUT(range32(address));

		return out;
	}


	X86InstructionType JmpInstruction::itype() const
	{
		return X86InstructionType::JMP;
	}

	ByteRange JmpInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// EB cb --> JMP rel8
			PUT(range8(0xEB));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// E9 cd --> JMP rel32
			PUT(range8(0xE9));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jmp", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JeInstruction::itype() const
	{
		return X86InstructionType::JE;
	}

	ByteRange JeInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 74 cb --> JE rel8
			PUT(range8(0x74));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 84 cd --> JE rel32
			PUT(range8(0x0F));
			PUT(range8(0x84));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("je", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JneInstruction::itype() const
	{
		return X86InstructionType::JNE;
	}

	ByteRange JneInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 75 cb --> JNE rel8
			PUT(range8(0x75));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 85 cd --> JNE rel32
			PUT(range8(0x0F));
			PUT(range8(0x85));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jne", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JlInstruction::itype() const
	{
		return X86InstructionType::JL;
	}

	ByteRange JlInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 7C cb --> JL rel8
			PUT(range8(0x7C));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 8C cd --> JL rel32
			PUT(range8(0x0F));
			PUT(range8(0x8C));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jl", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JleInstruction::itype() const
	{
		return X86InstructionType::JLE;
	}

	ByteRange JleInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 7E cb --> JLE rel8
			PUT(range8(0x7E));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 8E cd --> JLE rel32
			PUT(range8(0x0F));
			PUT(range8(0x8E));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jle", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JgInstruction::itype() const
	{
		return X86InstructionType::JG;
	}

	ByteRange JgInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 7F cb --> JG rel8
			PUT(range8(0x7F));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 8F cd --> JG rel32
			PUT(range8(0x0F));
			PUT(range8(0x8F));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jg", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JgeInstruction::itype() const
	{
		return X86InstructionType::JGE;
	}

	ByteRange JgeInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 7D cb --> JGE rel8
			PUT(range8(0x7D));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));

		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 8D cd --> JGE rel32
			PUT(range8(0x0F));
			PUT(range8(0x8D));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jge", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JaInstruction::itype() const
	{
		return X86InstructionType::JA;
	}

	ByteRange JaInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 77 cb --> JA rel8
			PUT(range8(0x77));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 87 cd --> JA rel32
			PUT(range8(0x0F));
			PUT(range8(0x87));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("ja", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JaeInstruction::itype() const
	{
		return X86InstructionType::JAE;
	}

	ByteRange JaeInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 73 cb --> JAE rel8
			PUT(range8(0x73));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 83 cd --> JAE rel32
			PUT(range8(0x0F));
			PUT(range8(0x83));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jae", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JbInstruction::itype() const
	{
		return X86InstructionType::JB;
	}

	ByteRange JbInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 72 cb --> JB rel8
			PUT(range8(0x72));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 82 cd --> JB rel32
			PUT(range8(0x0F));
			PUT(range8(0x82));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jb", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType JbeInstruction::itype() const
	{
		return X86InstructionType::JBE;
	}

	ByteRange JbeInstruction::emit() const
	{
		ByteRange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			// 76 cb --> JBE rel8
			PUT(range8(0x76));
			PUT(range8(std::bit_cast<std::uint8_t>(ei.to_integral<std::int8_t>())));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// 0F 86 cd --> JBE rel32
			PUT(range8(0x0F));
			PUT(range8(0x86));
			PUT(range32(std::bit_cast<std::uint32_t>(ei.to_integral<std::int32_t>())));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jbe", ei.to_string());
			return {};
		}

		return out;
	}


	X86InstructionType SeteInstruction::itype() const
	{
		return X86InstructionType::SETE;
	}

	ByteRange SeteInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 94 --> SETE r/m8
				PUT(range8(0x0F));
				PUT(range8(0x94));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("sete", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetneInstruction::itype() const
	{
		return X86InstructionType::SETNE;
	}

	ByteRange SetneInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 95 --> SETNE r/m8
				PUT(range8(0x0F));
				PUT(range8(0x95));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setne", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetlInstruction::itype() const
	{
		return X86InstructionType::SETL;
	}

	ByteRange SetlInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 9C --> SETL r/m8
				PUT(range8(0x0F));
				PUT(range8(0x9C));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setl", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetleInstruction::itype() const
	{
		return X86InstructionType::SETLE;
	}

	ByteRange SetleInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 9E --> SETLE r/m8
				PUT(range8(0x0F));
				PUT(range8(0x9E));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setle", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetgInstruction::itype() const
	{
		return X86InstructionType::SETG;
	}

	ByteRange SetgInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 9F --> SETG r/m8
				PUT(range8(0x0F));
				PUT(range8(0x9F));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setg", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetgeInstruction::itype() const
	{
		return X86InstructionType::SETGE;
	}

	ByteRange SetgeInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 9D --> SETGE r/m8
				PUT(range8(0x0F));
				PUT(range8(0x9D));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setge", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetaInstruction::itype() const
	{
		return X86InstructionType::SETA;
	}

	ByteRange SetaInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 97 --> SETA r/m8
				PUT(range8(0x0F));
				PUT(range8(0x97));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("seta", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetaeInstruction::itype() const
	{
		return X86InstructionType::SETAE;
	}

	ByteRange SetaeInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 93 --> SETAE r/m8
				PUT(range8(0x0F));
				PUT(range8(0x93));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setae", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetbInstruction::itype() const
	{
		return X86InstructionType::SETB;
	}

	ByteRange SetbInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 92 --> SETB r/m8
				PUT(range8(0x0F));
				PUT(range8(0x92));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setb", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SetbeInstruction::itype() const
	{
		return X86InstructionType::SETBE;
	}

	ByteRange SetbeInstruction::emit() const
	{
		using enum X86OperandType;
		switch (operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(operand.get());
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				ByteRange out{};

				// 0F 96 --> SETBE r/m8
				PUT(range8(0x0F));
				PUT(range8(0x96));
				PUT(range8(X86Builder::modrm_rr(destination, destination)));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("setbe", _operand_type_map.at(operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType NopInstruction::itype() const
	{
		return X86InstructionType::NOP;
	}

	ByteRange NopInstruction::emit() const
	{
		ByteRange out{};

		PUT(range8(0x90));

		return out;
	}


	X86InstructionType RetInstruction::itype() const
	{
		return X86InstructionType::RET;
	}

	ByteRange RetInstruction::emit() const
	{
		ByteRange out{};
		
		if (immediate != -1)
		{
			// C2 iw --> RET imm16
			PUT(range8(0xC2));
			PUT(range16(std::bit_cast<std::uint16_t>(immediate)));
		}
		else
		{
			// C3 --> RET
			PUT(range8(0xC3));
		}
		
		return out;
	}


	X86InstructionType LeaveInstruction::itype() const
	{
		return X86InstructionType::LEAVE;
	}

	ByteRange LeaveInstruction::emit() const
	{
		ByteRange out{};
		
		// C9 --> LEAVE
		PUT(range8(0xC9));
		
		return out;
	}
}
