#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include <command/CommandType.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze Command.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Command 
		: public InjectSingleton<ErrorReporter>
	{
	public:
		std::uint32_t offset;
		Token token;

	public:
		Command(Token token)
			: token{ std::move(token) }
		{
		}

	public:
		virtual CommandType ctype() const = 0;
	};
}

#endif
