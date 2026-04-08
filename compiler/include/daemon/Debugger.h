#ifndef HAZE_DEBUGGER_H
#define HAZE_DEBUGGER_H

#include <utility/PlatformVariables.h>

// Haze Debugger.h
// (c) Connor J. Link. All Rights Reserved.

// NOTE: the following declarations are implemented in Rust and linked dynamically
namespace hz
{
	extern "C" 
	{
		// debug server
		__declspec(dllimport) void mist_initialize(const char* connection_string);
		__declspec(dllimport) void mist_launch_target(const char* target_path);
		
		// breakpoint subsystem
		__declspec(dllimport) void mist_clear_function_symbols();
		__declspec(dllimport) void mist_register_function_symbol_rva(const char* name, native_uint address);
		__declspec(dllimport) void mist_register_function_symbol_va(const char* name, native_uint address);
	}
}

#endif
