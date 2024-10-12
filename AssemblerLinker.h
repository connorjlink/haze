#ifndef HAZE_ASSEMBLERLINKER_H
#define HAZE_ASSEMBLERLINKER_H

#include "Linker.h"
#include "AssemblerParser.h"
#include "InstructionCommand.h"

// Haze AssemblerLinker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AssemblerLinker : public Linker
	{
	private:
		std::vector<Node*> commands;
		AssemblerParser* assembler_parser;

	public:
		std::uint32_t approximate_size()
		{
			// This is a very rough estimation of the number of bytes required by an inline
			// assembly block that doesn't utilize .org directives to jump around. In that case,
			// our linking doesn't really work. For now I am fine with this limitation.
			return 3 * static_cast<std::uint32_t>(commands.size());
		}

	public:
		AssemblerLinker(std::vector<Node*>&& commands, AssemblerParser* assembler_parser, const std::string& filepath)
			: Linker{ filepath }, commands { std::move(commands) }, assembler_parser{ assembler_parser }
		{
			commands.erase(std::remove_if(commands.begin(), commands.end(), [&](auto&& val)
			{
				if (val->ntype() != NodeType::COMMAND)
				{
					if (!_node_type_map.contains(val->ntype()))
					{
						_error_reporter->post_error("unrecognized node type", NULL_TOKEN);
					}

					else
					{
						_error_reporter->post_error(std::format("invalid node type `{}`", _node_type_map.at(val->ntype())), NULL_TOKEN);
					}

					return true;
				}

				return false;
			}));
		}

	public:
		virtual LinkerType ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<InstructionCommand*> link(std::uint32_t) final override;
	};
}

#endif
