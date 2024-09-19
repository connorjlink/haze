#ifndef HAZE_BINARYCONSTANTS_H
#define HAZE_BINARYCONSTANTS_H

// Haze BinaryConstants.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// for magic
	inline static constexpr auto DOS_HEADER_OFFSET = 0x00;
	// for lfanew
	inline static constexpr auto DOS_FOOTER_OFFSET = 0x30;


	// NOTE: pe and optional header immediately adjacent in the final image
	inline static constexpr auto PE_HEADER_OFFSET = 0x40;
	inline static constexpr auto OPTIONAL_HEADER_OFFSET = 0x58;


	// TODO: at offset 0xB8?
	// points to the imports VA
	inline static constexpr auto DATA_DIRECTORIES_OFFSET = 0xB8;


	inline static constexpr auto SECTIONS_TABLE_OFFSET = 0x138;
	// sections
	inline static constexpr auto CODE_OFFSET = 0x200; // .text
	inline static constexpr auto IMPORTS_OFFSET = 0x400; // .rdata
	inline static constexpr auto DATA_OFFSET = 0x600; // .data


	inline static constexpr auto IMAGE_SIZE = 0x4000;


	inline static constexpr auto PROCEDURE_BASE = 0x00402000;
#define PROCEDURE(x) (PROCEDURE_BASE + x)

	inline static constexpr auto STDOUT_HANDLE = 0x004032F0;

	inline static constexpr auto COPYRIGHT_LOGO = 0x00403000;


	// GetStdHandle()
	inline const auto getstdhandle_int_va = 0xA0;
	inline const auto getstdhandle_iat_va = 0xB0;

	// WriteConsole()
	inline const auto writeconsole_int_va = 0xC0;
	inline const auto writeconsole_iat_va = 0xD0;

	// ExitProcess()
	inline const auto exitprocess_int_va = 0xE0;
	inline const auto exitprocess_iat_va = 0xF0;

	// MessageBoxA()
	inline const auto messageboxa_int_va = 0x100;
	inline const auto messageboxa_iat_va = 0x110;


	// wsprintf()
	inline const auto wnsprintfa_int_va = 0x120;
	inline const auto wnsprintfa_iat_va = 0x130;


	// dll name pointers
	inline const auto kernel32_pointer = 0x150;
	inline const auto user32_pointer = 0x160;
	inline const auto shlwapi_pointer = 0x170;


	// function name strings
	inline const auto getstdhandle_va = 0x180;
	inline const auto writeconsole_va = 0x190;
	inline const auto exitprocess_va = 0x1A0;
	inline const auto messageboxa_va = 0x1B0;
	inline const auto wnsprintfa_va = 0x1C0;

	// dll name strings
	inline const auto kernel32_va = 0x1D0;
	inline const auto user32_va = 0x1E0;
	inline const auto shlwapi_va = 0x1F0;
}

#endif 
