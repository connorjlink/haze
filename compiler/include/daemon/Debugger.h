#ifndef HAZE_DEBUGGER_H
#define HAZE_DEBUGGER_H

// Haze Debugger.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using Address = std::uint32_t;

	// debug server
	extern "C" void mist_initialize(const char* connection_string);
	extern "C" void mist_launch_target(const char* target_path);

	// breakpoint subsystem
	extern "C" void mist_clear_function_symbols();
	extern "C" void mist_register_function_symbol_rva(const char* name, Address address);
	extern "C" void mist_register_function_symbol_va(const char* name, Address address);
}

#endif
