#include "Hook.h"
#include "Log.h"

namespace hz
{
	void Hook::hook()
	{
		Log::info("Hook set");
	}

	void Hook::unhook()
	{
		Log::info("Hook released");
	}
}