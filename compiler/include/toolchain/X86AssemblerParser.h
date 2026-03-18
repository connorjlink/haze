#ifndef HAZE_X86ASSEMBLERPARSER_H
#define HAZE_X86ASSEMBLERPARSER_H

#include <toolchain/AssemblerParser.h>

// Haze X86AssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86AssemblerParser : public AssemblerParser
	{
	public:
		virtual AssemblerParserType atype() const final override;

	private:
		virtual Node* parse_instruction_command() const final override;
		virtual register_t parse_register() const final override;

	public:
		using AssemblerParser::AssemblerParser;
	};
}

#endif
