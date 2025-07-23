import std;

#include <daemon/Debugger.h>

#include <tlhelp32.h>
#include <debugapi.h>
#include <psapi.h>

// Haze Debugger.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	bool is_invalid_handle(HANDLE handle)
	{
		return handle == nullptr || handle == INVALID_HANDLE_VALUE;
	}

	void* compute_target_address(void* image_base, DWORD rva)
	{
		return static_cast<BYTE*>(image_base) + rva;
	}
}

namespace hz
{
	HANDLE Debugger::get_process_handle(const std::wstring& name, DWORD desired_access = PROCESS_ALL_ACCESS)
	{
		auto process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (::is_invalid_handle(process_snapshot))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"failed to create process snapshot for `{}`", name), NULL_TOKEN);
			return nullptr;
		}

		PROCESSENTRY32W pe32{};
		pe32.dwSize = sizeof(PROCESSENTRY32W);

		if (!Process32FirstW(process_snapshot, &pe32))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"failed to retrieve first snapshot process for `{}`", name), NULL_TOKEN);
			CloseHandle(process_snapshot);
			return nullptr;
		}

		HANDLE process = nullptr;
		do 
		{
			if (_wcsicmp(pe32.szExeFile, name.c_str()) == 0) 
			{
				process = OpenProcess(desired_access, FALSE, pe32.th32ProcessID);
				break;
			}
		} while (Process32NextW(process_snapshot, &pe32));

		CloseHandle(process_snapshot);
		return process;
	}

	void* Debugger::resolve_image_base(HANDLE process)
	{
		// this is probably overkil, but this function only gets called once per debug attach, so it is probably okay
		HMODULE modules[1024] = { 0 };
		DWORD cbNeeded;

		if (EnumProcessModules(process, modules, sizeof(modules), &cbNeeded))
		{
			MODULEINFO modInfo{};
			if (GetModuleInformation(process, modules[0], &modInfo, sizeof(modInfo)))
			{
				return modInfo.lpBaseOfDll;
			}
		}

		return nullptr;
	}

	void Debugger::attach_to_process(const std::wstring& name)
	{
		if (_process_handle != nullptr)
		{
			USE_SAFE(ErrorReporter)->post_error("debugger is already attached to a process", NULL_TOKEN);
			return;
		}

		_process_handle = get_process_handle(name);
		if (::is_invalid_handle(_process_handle))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"failed to obtain a valid process handle for `{}`", name), NULL_TOKEN);
			return;
		}

		_image_base = resolve_image_base(_process_handle);
		if (_image_base == nullptr)
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"failed to resolve image base for process `{}`", name), NULL_TOKEN);
			CloseHandle(_process_handle);
			_process_handle = nullptr;
			return;
		}

		DebugActiveProcess(GetProcessId(_process_handle));

		// NOTE: liaisons only with the entry thread of the process
		const auto thread_id = await_get_thread_id(_process_handle);
		_thread_handle = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);
		if (::is_invalid_handle(_thread_handle))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"failed to open thread handle for thread id `{}` in process `{}`", thread_id, name), NULL_TOKEN);
			CloseHandle(_process_handle);
			_process_handle = nullptr;
			return;
		}
	}

	DWORD Debugger::await_get_thread_id(HANDLE process)
	{
		// wait indefinitely until the process triggers a debug event so the debugger can read a valid thread handle
		DEBUG_EVENT debug_event{};
		WaitForDebugEvent(&debug_event, INFINITE);

		if (debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT && 
			debug_event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
		{
			return debug_event.dwThreadId;
		}

		return DWORD{ 0 };
	}

	bool Debugger::inject_software_breakpoint_at(DWORD rva, BYTE& original)
	{
		static constexpr BYTE int3 = 0xCC;

		SIZE_T bytes_read{}, bytes_written{};

		void* target_address = ::compute_target_address(_image_base, rva);

		if (!ReadProcessMemory(_process_handle, target_address, &original, 1, &bytes_read) || bytes_read != 1)
			return false;

		if (!WriteProcessMemory(_process_handle, target_address, &int3, 1, &bytes_written) || bytes_written != 1)
			return false;

		// must flush cache to ensure breakpoint is active
		FlushInstructionCache(_process_handle, target_address, 1);

		return true;
	}

	bool Debugger::inject_hardware_breakpoint_at(DWORD rva, BYTE& original)
	{
		CONTEXT context{};
		context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
		GetThreadContext(_thread_handle, &context);

		const auto target = ::compute_target_address(_image_base, rva);

		context.Dr0 = reinterpret_cast<DWORD_PTR>(target);
		// local enable for DR0 hardware breakpoint
		context.Dr7 = (context.Dr7 & ~0xF) | 0x1;  

		SetThreadContext(_thread_handle, &context);
	}
}
