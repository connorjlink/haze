#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "InstructionCommand.h"
#include "ErrorReporter.h"
#include "LinkerType.h"

#include <string>
#include <vector>
#include <cstdint>

namespace hz
{
	class Linker
	{
	public:
		Linker(const std::string& filepath)
		{
			_error_reporter->open_context(filepath, "linking");
		}

		virtual ~Linker()
		{
			_error_reporter->close_context();
		}

	public:
		virtual LinkerType ltype() const = 0;
		virtual bool optimize() = 0;
		virtual std::vector<InstructionCommand*> link(std::uint32_t) = 0;
	};

	extern Linker* _linker;
}

#endif
