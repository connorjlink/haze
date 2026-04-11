#ifndef HAZE_X86OPERANDTYPE_H
#define HAZE_X86OPERANDTYPE_H

// Haze X86OperandType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_IMMEDIATE_OPERAND(x) static_cast<x86::ImmediateOperand*>(x)
#define AS_INDIRECT_OPERAND(x) static_cast<x86::IndirectOperand*>(x)
#define AS_REGISTER_OPERAND(x) static_cast<x86::RegisterOperand*>(x)
#define AS_REGISTER_INDIRECT_OPERAND(x) static_cast<x86::RegisterIndirectOperand*>(x)
#define AS_REGISTER_DISPLACED_OPERAND(x) static_cast<x86::RegisterDisplacedOperand*>(x)

namespace hz
{
	enum class X86OperandType
	{
		IMMEDIATE,
		INDIRECT,
		REGISTER,
		REGISTER_INDIRECT,
		REGISTER_DISPLACED,
	};

	static const std::unordered_map<X86OperandType, std::string_view> _operand_type_map
	{
		{ X86OperandType::IMMEDIATE, "immediate" },
		{ X86OperandType::INDIRECT, "indirect" },
		{ X86OperandType::REGISTER, "register" },
		{ X86OperandType::REGISTER_INDIRECT, "register-indirect" },
		{ X86OperandType::REGISTER_DISPLACED, "register-displaced" },
	};
}

#endif
