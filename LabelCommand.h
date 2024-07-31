#ifndef HAZE_LABELCOMMAND_H
#define HAZE_LABELCOMMAND_H

#include "Command.h"

namespace hz
{
	class LabelCommand : public Command
	{
	public:
		std::string identifier;
		std::uint16_t address;

	public:
		LabelCommand(std::string identifier)
			: identifier(std::move(identifier))
		{
		}

	public:
		virtual CommandType ctype() const final override;
		virtual std::string string() const final override;
		virtual LabelCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual LabelCommand* optimize() final override;
	};
}

#endif
