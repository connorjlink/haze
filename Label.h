#ifndef HAZE_LABEL_H
#define HAZE_LABEL_H

#include "Command.h"

namespace hz
{
	class Label : public Command
	{
	private:
		std::string identifier;

	public:
		Label(std::string identifier)
			: identifier(std::move(identifier))
		{
		}

	public:
		virtual Command::Type ctype() const final override;
		virtual std::string string() const final override;
		virtual Label* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Label* optimize() final override;
	};
}

#endif
