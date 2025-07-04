#ifndef HAZE_ASSEMBLERLINKER_H
#define HAZE_ASSEMBLERLINKER_H

#include <toolchain/Linker.h>

// Haze AssemblerLinker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AssemblerParser;

	class AssemblerLinker : public Linker
	{
	private:
		std::vector<Node*> commands;
		AssemblerParser* assembler_parser;

	public:
		inline std::uint32_t approximate_size() const
		{
			// This is a very rough estimation of the number of bytes required by an inline
			// assembly block that doesn't utilize .org directives to jump around. In that case,
			// our linking doesn't really work. For now I am fine with this limitation.
			return 4 * static_cast<std::uint32_t>(commands.size());
		}

	public:
		AssemblerLinker(std::vector<Node*>&&, AssemblerParser*, const std::string&);

	public:
		virtual LinkerType ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<InstructionCommand*> link(native_int, native_int) final override;
	};
}

#endif
