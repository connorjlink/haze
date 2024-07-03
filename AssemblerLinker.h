#ifndef HAZE_ASSEMBLERLINKER_H
#define HAZE_ASSEMBLERLINKER_H

#include "Linker.h"

#include "AssemblerParser.h"

#include "Command.h"

#include "Log.h"

namespace hz
{
	class AssemblerLinker : public Linker
	{
	private:
		std::vector<Node*> commands;
		AssemblerParser* assembler_parser;

	public:
		std::uint16_t approximate_size()
		{
			// This is a very rough estimation of the number of bytes required by an inline
			// assembly block that doesn't utilize .org directives to jump around. In that case,
			// our linking doesn't really work. For now I am fine with this limitation.
			return 3 * static_cast<std::uint16_t>(commands.size());
		}

	public:
		AssemblerLinker(std::vector<Node*>&& commands, AssemblerParser* assembler_parser)
			: commands{ std::move(commands) }, assembler_parser{ assembler_parser }
		{
			for (auto node : commands)
			{
				if (node->ntype() != Node::Type::COMMAND)
				{
					Log::error("Invalid node type encountered for assembly linking");
				}
			}
		}

	public:
		virtual Linker::Type ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<std::uint8_t> link(std::uint16_t) final override;
	};
}

#endif
