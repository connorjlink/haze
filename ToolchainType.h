#ifndef HAZE_TOOLCHAIN_TYPE
#define HAZE_TOOLCHAIN_TYPE

#define AS_ASSEMBLER_TOOLCHAIN(x) static_cast<AssemblerToolchain*>(x)
#define AS_COMPILER_TOOLCHAIN(x) static_cast<CompilerToolchain*>(x)
#define AS_INTERPRETER_TOOLCHAIN(x) static_cast<InterpreterToolchain*>(x)

namespace hz
{
	enum class ToolchainType
	{
		ASSEMBLER,
		COMPILER,
		INTERPRETER,
	};
}

#endif 
