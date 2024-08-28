#ifndef HAZE_COMPILERLINKER_H
#define HAZE_COMPILERLINKER_H

#include "Linker.h"

#include <vector>
#include <cstdint>

namespace hz
{
	class CompilerLinker : public Linker
	{
	private:
		std::vector<Linkable> linkables;

	public:
		CompilerLinker(std::vector<Linkable>&& linkables, const std::string& filepath)
			: Linker{ filepath }, linkables { std::move(linkables) }
		{
		}

	public:   
		virtual LinkerType ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<InstructionCommand*> link(std::uint32_t) final override;
	};
}

#endif
