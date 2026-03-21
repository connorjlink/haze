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
	X86InstructionType PushInstruction::itype() const
	{
		return X86InstructionType::PUSH;
	}

	byterange PushInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case IMMEDIATE:
			{
				const auto immediate_operand = AS_IMMEDIATE_OPERAND(_operand);
				const auto immediate = immediate_operand->_immediate;

				// NOTE: 8- and 16-bits are implicitly sign-extended to 32-bits when executed
				if (immediate.is_within_range<std::int8_t>())
				{
					byterange out{};

					// 6A --> PUSH imm8
					PUT(range8(0x6A));
					PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

					return out;
				} 
				else if (immediate.is_within_range<std::int32_t>())
				{
					byterange out{};

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
				const auto indirect_operand = AS_INDIRECT_OPERAND(_operand);
				const auto address = indirect_operand->_address;

				byterange out{};

				// FF /6 --> PUSH r/m32
				PUT(range8(0xFF));
				PUT(range8(0x35));
				PUT(range32(address));

				return out;
			} break;

			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(_operand);
				const auto source = VERIFY_REGISTER(register_operand->_register);

				// 50+rd --> PUSH r32
				return { static_cast<std::uint8_t>(0x50 | source) };
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("push", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType PopInstruction::itype() const
	{
		return X86InstructionType::POP;
	}

	byterange PopInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(_operand);
				const auto destination = VERIFY_REGISTER(register_operand->_register);

				// 58+ rd --> POP r32
				return { static_cast<std::uint8_t>(0x58 | destination) };
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("pop", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType MovInstruction::itype() const
	{
		return X86InstructionType::MOV;
	}

	byterange MovInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			// dereferencing pointer
			case INDIRECT:
			{
				const auto indirect_destination = AS_INDIRECT_OPERAND(_destination);

				switch (_source->otype())
				{
					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = indirect_destination->_address;
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
						const auto indirect_source = AS_INDIRECT_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = indirect_source->_address;

						byterange out{};

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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

						// 89 /r --> MOV r/m32, r32
						PUT(range8(0x89));
						PUT(range8(X86Builder::modrm_rr(destination, source)));

						return out;
					} break;

					case REGISTER_DISPLACED:
					{
						const auto register_displaced_source = AS_REGISTER_DISPLACED_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_displaced_source->_register);
						const auto displacement = register_displaced_source->_displacement;

						byterange out{};

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
				const auto register_displaced_destination = AS_REGISTER_DISPLACED_OPERAND(_destination);
				
				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_displaced_destination->_register);
						const auto displacement = register_displaced_destination->_displacement;
						const auto immediate = immediate_source->_immediate;

						if (!EI(static_cast<std::intmax_t>(displacement)).is_within_range<std::int8_t>())
						{
							CommonErrors::unsupported_instruction_range("mov", std::to_string(displacement));
							return {};
						}


						if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
				CommonErrors::unsupported_instruction_format("mov", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType MovzxInstruction::itype() const
	{
		return X86InstructionType::MOVZX;
	}

	byterange MovzxInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("movzx", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType AddInstruction::itype() const
	{
		return X86InstructionType::ADD;
	}

	byterange AddInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								byterange out{};

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
							byterange out{};
							
							// 83 /0 ib --> ADD r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b000, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));
							
							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};
							
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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("add", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType SubInstruction::itype() const
	{
		return X86InstructionType::SUB;
	}

	byterange SubInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								byterange out{};

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
							byterange out{};

							// 83 /5 ib --> SUB r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b101, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("sub", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType OrInstruction::itype() const
	{
		return X86InstructionType::OR;
	}

	byterange OrInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								byterange out{};

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
							byterange out{};

							// 83 /1 ib --> OR r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b001, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("or", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType AndInstruction::itype() const
	{
		return X86InstructionType::AND;
	}

	byterange AndInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								byterange out{};

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
							byterange out{};

							// 83 /4 ib --> AND r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("and", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType XorInstruction::itype() const
	{
		return X86InstructionType::XOR;
	}

	byterange XorInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								byterange out{};

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
							byterange out{};

							// 83 /6 ib --> XOR r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b110, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("xor", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType IncInstruction::itype() const
	{
		return X86InstructionType::INC;
	}

	byterange IncInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(_operand);
				const auto source = VERIFY_REGISTER(register_operand->_register);

				byterange out{};

				// 40+rd --> INC r32
				PUT(range8(0x40 | source));

				return out;
			} break;
			
			default:
			{
				CommonErrors::unsupported_instruction_format("inc", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType DecInstruction::itype() const
	{
		return X86InstructionType::DEC;
	}

	byterange DecInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_operand->otype())
		{
			case REGISTER:
			{
				const auto register_operand = AS_REGISTER_OPERAND(_operand);
				const auto source = VERIFY_REGISTER(register_operand->_register);

				byterange out{};

				// 48+rd --> DEC r32
				PUT(range8(0x48 | source));

				return out;
			} break;

			default:
			{
				CommonErrors::unsupported_instruction_format("dec", _operand_type_map.at(_operand->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType TestInstruction::itype() const
	{
		return X86InstructionType::TEST;
	}

	byterange TestInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case REGISTER:
					{
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("test", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType CmpInstruction::itype() const
	{
		return X86InstructionType::CMP;
	}

	byterange CmpInstruction::emit() const
	{
		using enum X86OperandType;
		switch (_destination->otype())
		{
			case REGISTER:
			{
				const auto register_destination = AS_REGISTER_OPERAND(_destination);

				switch (_source->otype())
				{
					case IMMEDIATE:
					{
						const auto immediate_source = AS_IMMEDIATE_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto immediate = immediate_source->_immediate;

						if (destination == EAX)
						{
							if (immediate.is_within_range<std::int32_t>())
							{
								byterange out{};

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
							byterange out{};

							// 83 /7 ib --> CMP r/m32, imm8
							PUT(range8(0x83));
							PUT(range8(X86Builder::modrm(0b11, 0b111, destination)));
							PUT(range8(std::bit_cast<std::uint8_t>(immediate.to_integral<std::int8_t>())));

							return out;
						}
						else if (immediate.is_within_range<std::int32_t>())
						{
							byterange out{};

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
						const auto register_source = AS_REGISTER_OPERAND(_source);
						const auto destination = VERIFY_REGISTER(register_destination->_register);
						const auto source = VERIFY_REGISTER(register_source->_register);

						byterange out{};

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
				CommonErrors::unsupported_instruction_format("cmp", _operand_type_map.at(_destination->otype()));
				return {};
			} break;
		}
	}


	X86InstructionType CallInstruction::itype() const
	{
		return X86InstructionType::CALL;
	}

	byterange CallInstruction::emit() const
	{
		byterange out{};


		return out;
	}


	X86InstructionType ApicallInstruction::itype() const
	{
		return X86InstructionType::APICALL;
	}

	byterange ApicallInstruction::emit() const
	{
		byterange out{};


		return out;
	}


	X86InstructionType JmpInstruction::itype() const
	{
		return X86InstructionType::JMP;
	}

	byterange JmpInstruction::emit() const
	{
		byterange out{};

		const auto ei = EI(static_cast<std::intmax_t>(_displacement));

		if (ei.is_within_range<std::int8_t>())
		{
			//EB cb --> JMP rel8
			PUT(range8(0xEB));
			PUT(range8(_displacement));
		}
		else if (ei.is_within_range<std::int32_t>())
		{
			// E9 cd --> JMP rel32
			PUT(range8(0xE9));
			PUT(range32(_displacement));
		}
		else
		{
			CommonErrors::unsupported_instruction_range("jmp", ei.to_string());
			return {};
		}

		return out;
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
