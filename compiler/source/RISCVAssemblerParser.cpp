import std;

#include <toolchain/RISCVAssemblerParser.h>
#include <riscv/RISCVInstruction.h>
#include <riscv/defs/RISCVRegister.h>

// Haze RISCVAssemblerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

#define PARSE_REGISTER() static_cast<RISCVRegister>(parse_register())

namespace hz
{
	using namespace riscv;

#pragma message("TODO: implement RISC-V assembly parsing instructions")

	CommandReference<InstructionCommand> RISCVAssemblerParser::parse_instruction_command_implementation()
	{
		const auto& opcode_token = peek();
		consume(opcode_token.kind);

		using enum TokenKind;
		switch (opcode_token.kind)
		{
#define X(enumerator, name) case TokenKind::enumerator: return parse_##enumerator##_instruction();
			RISCV_INSTRUCTION_KINDS(X)
#undef X
		}
		
		USE_SAFE(ErrorReporter)->post_error(std::format(
			"unrecognized instruction opcode `{}`", opcode_token.text), opcode_token);
		return MAKE_INVALID_REFERENCE(InstructionCommand, Command, command_storage);
	}

	Register RISCVAssemblerParser::parse_register_implementation()
	{
		const auto& register_token = peek();
		consume(register_token.kind);

		switch (register_token.kind)
		{
#define X(enumerator, name, value, category) case TokenKind::enumerator: return to_register(RISCVRegister::enumerator);
			RISCV_REGISTERS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"unrecognized RISC-V register `{}`", register_token.text), register_token);
		return Register{ -1 };
	}
}
