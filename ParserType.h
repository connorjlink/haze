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

	static const std::unordered_map<ParserType, std::string> _parser_type_map
	{
		{ ParserType::COMPILER, "compiler" },
		{ ParserType::ASSEMBLER, "assembler" },
		{ ParserType::INTERPRETER, "interpreter" },
	};
}

#endif 
