#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include <ast/Node.h>
#include <command/CommandType.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze Command.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Command 
		: public Node
		, public InjectSingleton<ErrorReporter>
	{
	public:
		std::uint32_t offset;

	public:
		Command(Token token)
			: Node{ token }
		{
		}

	public:
		virtual NodeType ntype() const final override;
		virtual CommandType ctype() const = 0;
	};
}

#endif
