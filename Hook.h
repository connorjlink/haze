#ifndef HAZE_HOOK_H
#define HAZE_HOOK_H

#include "HookTopicBuilder.h"
#include "HookClient.h"

namespace hz
{
	class Hook
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
