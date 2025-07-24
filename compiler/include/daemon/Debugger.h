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
#ifdef __cplusplus
	extern "C"
	{
#endif

		HANDLE get_process_handle(PWSTR, DWORD);
		void* resolve_image_base(HANDLE);
		DWORD await_get_thread_id(HANDLE);

		void attach_to_process(PWSTR);
		bool inject_software_breakpoint_at(DWORD, PBYTE);
		bool inject_hardware_breakpoint_at(DWORD, PBYTE);
		bool toggle_single_step(void);

		class Debugger
			: public SingletonTag<Debugger>
			, public InjectSingleton<ErrorReporter>
		{
		private:
			HANDLE get_process_handle(const std::wstring&, DWORD);
			void* resolve_image_base(HANDLE);
			DWORD await_get_thread_id(HANDLE);

		private:
			DWORD _thread_id = 0;
			DWORD _process_id = 0;
			HANDLE _process_handle = nullptr;
			// for now, the compiler runtime ever runs one thread in its process
			HANDLE _thread_handle = 0;
			void* _image_base = nullptr;

		public:
			void attach_to_process(const std::wstring&);
			bool inject_software_breakpoint_at(DWORD, BYTE&);
			bool inject_hardware_breakpoint_at(DWORD, BYTE&);
			bool set_single_step(void);
#warning TODO: Implement hardware context schedulding (effective USING_CONTEXT_SAFE with a callable)
#warning TODO: implement state transformations with a source map to enable step in/out (reverse source position to a memory address in the corresponding function)
		};

#ifdef __cplusplus
	}
#endif
}

#endif
