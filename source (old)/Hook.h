#ifndef HAZE_HOOK_H
#define HAZE_HOOK_H

#include "HookTopicBuilder.h"
#include "HookClient.h"
#include "DependencyInjector.h"
#include "ErrorReporter.h"

// Haze Hook.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Hook
		: public InjectSingleton<ErrorReporter>
	{
	private:
		// MQTT client interface
		HookClient _client;

	public:
		void hook();
		void unhook();

	public:
		Hook()
			: _client{ "localhost", "1883", "haze-interpreter-hook" }
		{
		}
	};
}

#endif
