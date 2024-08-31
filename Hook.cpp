#include "Hook.h"
#include "ErrorReporter.h"

// Haze Hook.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Hook::hook()
	{
		_error_reporter->post_information("hook set", NULL_TOKEN);
	}

	void Hook::unhook()
	{
		_error_reporter->post_information("hook released", NULL_TOKEN);
	}
}