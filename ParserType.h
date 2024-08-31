#ifndef HAZE_PARSERTYPE_H
#define HAZE_PARSERTYPE_H

// Haze ParserType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_ASSEMBLER_PARSER(x) static_cast<AssemblerParser*>(x)
#define AS_COMPILER_PARSER(x) static_cast<CompilerParser*>(x)
#define AS_INTERPRETER_PARSER(x) static_cast<InterpreterParser*>(x)

namespace hz
{
	enum class ParserType
	{
		COMPILER,
		ASSEMBLER,
		INTERPRETER,
	};
}

#endif 
