#ifndef HAZE_TOOLCHAIN_TYPE
#define HAZE_TOOLCHAIN_TYPE

#define AS_ASSEMBLER_TOOLCHAIN(x) static_cast<AssemblerToolchain*>(x)
#define AS_COMPILER_TOOLCHAIN(x) static_cast<CompilerToolchain*>(x)
#define AS_INTERPRETER_TOOLCHAIN(x) static_cast<InterpreterToolchain*>(x)

#include <string>
#include <unordered_map>

// Haze ToolchainType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ToolchainType
	{
		ASSEMBLER,
		COMPILER,
		INTERPRETER,
	};

	static const std::unordered_map<std::string, ToolchainType> _toolchain_map
	{
		{ ".hzs", ToolchainType::ASSEMBLER },
		{ ".hz", ToolchainType::COMPILER },
		{ ".hzi", ToolchainType::INTERPRETER },
	};
}

#endif 
