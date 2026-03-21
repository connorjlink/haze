#ifndef HAZE_COMPILERLINKER_H
#define HAZE_COMPILERLINKER_H

#include <toolchain/Linker.h>
#include <toolchain/Linkable.h>

// Haze CompilerLinker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CompilerLinker : public Linker
	{
	private:
		std::unordered_map<std::string, Linkable> _linkables;

	public:
		CompilerLinker(std::vector<Linkable>&& linkables, const std::string& filepath)
			: Linker{ filepath }
		{
			for (auto& linkable : linkables)
			{
				_linkables[linkable.symbol->name] = std::move(linkable);
			}
		}

	public:
		virtual LinkerType ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<InstructionCommand*> link(native_uint, native_uint) final override;
	};
}

#endif
