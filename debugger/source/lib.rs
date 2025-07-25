/*

HANDLE get_process_handle(PWSTR, DWORD);
void* resolve_image_base(HANDLE);
DWORD await_get_thread_id(HANDLE);

void attach_to_process(PWSTR);
bool inject_software_breakpoint_at(DWORD, PBYTE);
bool inject_hardware_breakpoint_at(DWORD, PBYTE);
bool toggle_single_step(void);

*/

use std::ptr::null_mut;
use windows::{
    core::PCWSTR,
    Win32::Foundation::{HANDLE, HMODULE, CloseHandle},
    Win32::System::Diagnostics::ToolHelp::{
        CreateToolhelp32Snapshot, Process32FirstW, Process32NextW, PROCESSENTRY32W, TH32CS_SNAPPROCESS,
    },
    Win32::System::ProcessStatus::{
        EnumProcessModules, GetModuleInformation, MODULEINFO,
    },
    Win32::System::Threading::{OpenProcess, PROCESS_ACCESS_RIGHTS, PROCESS_ALL_ACCESS},
    Win32::Globalization::{
        CompareStringOrdinal, CSTR_EQUAL,
    },
};

fn nullptr() -> *mut std::ffi::c_void {
    return null_mut();
}

fn compare_pcwstr_case_insensitive(a: PCWSTR, b: PCWSTR) -> bool {
    unsafe {
        match (a.to_string(), b.to_string()) {
            (Ok(sa), Ok(sb)) => sa.to_lowercase() == sb.to_lowercase(),
            _ => false,
        }
    }
}

struct Debugger {
    toolhelp_snapshot: HANDLE,

}

impl Drop for Debugger {
    fn drop(&mut self) {
        unsafe {
            _ = CloseHandle(self.toolhelp_snapshot);
        }
    }
}

fn get_process_handle(name: PCWSTR, desired_access: u32) -> HANDLE {
    unsafe {
        let snapshot_result = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if snapshot_result.is_err() {
            return HANDLE(nullptr());
        }
        let snapshot = snapshot_result.unwrap();

        let mut entry = PROCESSENTRY32W {
            dwSize: std::mem::size_of::<PROCESSENTRY32W>() as u32,
            ..Default::default()
        };

        let process = Process32FirstW(snapshot, &mut entry);
        if process.is_err() {
            _ = CloseHandle(snapshot);
            return HANDLE(nullptr());
        }

        let mut process_handle = HANDLE(nullptr());
        loop {
            let exe_name = PCWSTR(entry.szExeFile.as_ptr());
            if compare_pcwstr_case_insensitive(exe_name, name) {
                let access_rights = PROCESS_ACCESS_RIGHTS(desired_access);

                let opened_process = OpenProcess(access_rights, false, entry.th32ProcessID);
                if opened_process.is_err() {
                    _ = CloseHandle(snapshot);
                    return HANDLE(nullptr());
                }

                process_handle = opened_process.unwrap();
                break;
            }

            let next_process = Process32NextW(snapshot, &mut entry);
            if next_process.is_err() {
                break;
            }
        }

        _ = CloseHandle(snapshot);
        return process_handle;
    }
}

fn resolve_image_base(process: HANDLE) -> *mut std::ffi::c_void {
    unsafe {
        // this is probably overkil, but this function only gets called once per debug attach, so it is probably okay
        let mut modules = [HMODULE(nullptr()); 1024];
        let mut bytes = 0u32;

        let modules_result = EnumProcessModules(
            process,
            modules.as_mut_ptr(),
            std::mem::size_of_val(&modules) as u32,
            &mut bytes,
        );
        if modules_result.is_err() {
            return nullptr();
        }

        let mut mod_info = MODULEINFO::default();
        // always assuming the first module is the main executable
        let information_result = GetModuleInformation(
            process,
            modules[0],
            &mut mod_info,
            std::mem::size_of::<MODULEINFO>() as u32,
        );
        if information_result.is_err() {
            return nullptr();
        }

        return mod_info.lpBaseOfDll;
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn initialize_debugger(name: PCWSTR) {
    let process_handle = get_process_handle(name, PROCESS_ALL_ACCESS.0);
    if process_handle.is_invalid() {
        eprintln!("Failed to get process handle for {:?}", name);
        return;
    }

    // compute process Id

    // find main thread Id (blocking await)

    // compute thread handle

    // attach to process

    // resolve image base

    // allow breakpoint injection (software globally, hardware per thread with Dr0/Dr7)

    let debugger = Debugger {
        toolhelp_snapshot: process_handle,
    };
}
