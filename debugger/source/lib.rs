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
    Win32::Foundation::{HANDLE, CloseHandle},
    Win32::System::Diagnostics::ToolHelp::{
        CreateToolhelp32Snapshot, Process32FirstW, Process32NextW, PROCESSENTRY32W, TH32CS_SNAPPROCESS,
    },
    Win32::System::Threading::{OpenProcess, PROCESS_ALL_ACCESS},
    Win32::Globalization::{
        CompareStringOrdinal, CSTR_EQUAL,
    },
};

struct Debugger {
    toolhelp_snapshot: HANDLE,

}

impl Drop for Debugger {
    fn drop(&mut self) {
        unsafe {
            CloseHandle(self.toolhelp_snapshot);
        }
    }
}

fn nullptr() -> *mut std::ffi::c_void {
    return null_mut();
}

pub fn compare_pcwstr_case_insensitive(a: PCWSTR, b: PCWSTR) -> bool {
    unsafe {
        match (a.to_string(), b.to_string()) {
            (Ok(sa), Ok(sb)) => sa.to_lowercase() == sb.to_lowercase(),
            _ => false,
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn get_process_handle(name: PCWSTR, desired_access: u32) -> HANDLE {
    unsafe {
        let snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if snapshot.is_err() {
            return HANDLE(nullptr());
        }

        let mut entry = PROCESSENTRY32W {
            dwSize: std::mem::size_of::<PROCESSENTRY32W>() as u32,
            ..Default::default()
        };

        let process = Process32FirstW(snapshot.unwrap(), &mut entry);
        if process.is_err() {
            CloseHandle(snapshot.unwrap());
            return HANDLE(nullptr());
        }

        let mut process_handle = HANDLE(nullptr());
        loop {
            let exe_name = PCWSTR(entry.szExeFile.as_ptr());
            if windows::Win32::Globalization::_wcsicmp(exe_name, name) == 0 {
                let opened_process = OpenProcess(desired_access, false, entry.th32ProcessID);
                process_handle = OpenProcess(desired_access, false, entry.th32ProcessID);
                break;
            }

            let next_process = Process32NextW(snapshot.unwrap(), &mut entry);
            if next_process.is_err() {
                break;
            }
        }

        CloseHandle(snapshot.unwrap());
        return process_handle;
    }
}

