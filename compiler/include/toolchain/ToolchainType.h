#ifndef HAZE_TOOLCHAIN_TYPE
#define HAZE_TOOLCHAIN_TYPE

// Haze ToolchainType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_ASSEMBLER_TOOLCHAIN(x) static_cast<AssemblerToolchain*>(x)
#define AS_COMPILER_TOOLCHAIN(x) static_cast<CompilerToolchain*>(x)

namespace hz
{
	enum class ToolchainType
	{
		ASSEMBLER,
		COMPILER,
	};

	static const std::unordered_map<std::string, ToolchainType> _toolchain_map
	{
		{ ".hzs", ToolchainType::ASSEMBLER },
		{ ".hz", ToolchainType::COMPILER },
	};
}

#endif 
