#ifndef HAZE_RISCVASSEMBLERPARSER_H
#define HAZE_RISCVASSEMBLERPARSER_H

#include <toolchain/AssemblerParser.h>

// Haze RISCVAssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class RISCVAssemblerParser : public AssemblerParser
	{
	public:


	private:
		virtual Node* parse_instruction_command() final override;
		virtual register_t parse_register() final override;

	public:
		using AssemblerParser::AssemblerParser;
	};
}

#endif
