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
		CompilerLinker(std::vector<Linkable>&& linkables)
			: linkables{ std::move(linkables) }
		{
		}

	public:   
		virtual Linker::Type ltype() const final override;
		virtual bool optimize() final override;
		virtual std::vector<std::uint8_t> link() final override;
	};
}

#endif
