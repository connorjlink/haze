#ifndef HAZE_ASSEMBLERLINKER_H
#define HAZE_ASSEMBLERLINKER_H

#include "Linker.h"

#include "Command.h"

namespace hz
{
	class AssemblerLinker : public Linker
	{
	private:
		std::vector<Command*> commands;

	public:
		AssemblerLinker(std::vector<Command*>&& commands)
			: commands{ std::move(commands) }
		{
		}

	public:
		virtual Linker::Type ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<std::uint8_t> link() final override;
	};
}

#endif
