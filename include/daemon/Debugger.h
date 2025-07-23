#ifndef HAZE_DEBUGGER_H
#define HAZE_DEBUGGER_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef VOID // windows lol

// Haze Debugger.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Debugger
		: public SingletonTag<Debugger>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		HANDLE get_process_handle(const std::wstring&, DWORD);
		void* resolve_image_base(HANDLE);
		DWORD await_get_thread_id(HANDLE);

	private:
		HANDLE _process_handle = nullptr;
		// for now, the compiler runtime ever runs one thread in its process
		HANDLE _thread_handle = 0;
		void* _image_base = nullptr;

	public:
		void attach_to_process(const std::wstring&);
		bool inject_software_breakpoint_at(DWORD, BYTE&);
		bool inject_hardware_breakpoint_at(DWORD, BYTE&);
	};
}

#endif
