#ifndef HAZE_DEBUGGER_H
#define HAZE_DEBUGGER_H

#include <utility/PlatformVariables.h>

// Haze Debugger.h
// (c) Connor J. Link. All Rights Reserved.

// NOTE: the following declarations are implemented in Rust and linked dynamically
namespace hz
{
	// debug server
	extern "C" __declspec(dllimport) void mist_initialize(const char* connection_string);
	extern "C" __declspec(dllimport) void mist_launch_target(const char* target_path);

	// breakpoint subsystem
	extern "C" __declspec(dllimport) void mist_clear_function_symbols();
	extern "C" __declspec(dllimport) void mist_register_function_symbol_rva(const char* name, native_uint address);
	extern "C" __declspec(dllimport) void mist_register_function_symbol_va(const char* name, native_uint address);
}

#endif
