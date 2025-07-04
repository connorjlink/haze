#ifndef HAZE_BINARYCONSTANTS_H
#define HAZE_BINARYCONSTANTS_H

// Haze BinaryConstants.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum DOSHeader
	{
		// for magic
		DOS_HEADER_OFFSET = 0x00,
		// for lfanew
		DOS_FOOTER_OFFSET = 0x30,
	};

	enum PEHeader
	{
		// NOTE: pe and optional header immediately adjacent in the final image
		PE_HEADER_OFFSET       = 0x40,
		OPTIONAL_HEADER_OFFSET = 0x58,

		// TODO: at offset 0xB8?
		// points to the imports VA
		DATA_DIRECTORIES_OFFSET = 0xB8,
	};

	enum ImageData
	{
		SECTIONS_TABLE_OFFSET = 0x138,
		// sections
		CODE_OFFSET    = 0x200, // .text
		IMPORTS_OFFSET = 0x400, // .rdata
		DATA_OFFSET    = 0x600, // .data

		// total size in bytes, will be 16k padded anyway to conform to PE32+
		IMAGE_SIZE = 0x4000,
	};


	


	

	enum ConstantTable
	{
		// injected at runtime after calling GetStdHandle()
		STDOUT_HANDLE = 0x004032F0,

		PRINT_BUFFER = 0x004033F0,
		PRINT_BUFFER_SIZE = 0xF, // print() 15-char buffered

		COPYRIGHT_LOGO = 0x00403000,

		PROGRAM_OUTPUT_STRING = 0x0040304E,

		NUMBER_FORMAT_STRING = 0x0040305F,
	};
	


	inline static constexpr auto PROCEDURE_BASE = 0x00402000;
#define PROCEDURE(x) (PROCEDURE_BASE + x)

	

	

	enum VirtualAddress
	{
		// GetStdHandle()
		getstdhandle_int_va = 0xA0,
		getstdhandle_iat_va = 0xB0,

		// WriteConsole()
		writeconsole_int_va = 0xC0,
		writeconsole_iat_va = 0xD0,
		
		// ExitProcess()
		exitprocess_int_va = 0xE0,
		exitprocess_iat_va = 0xF0,

		// MessageBoxA()
		messageboxa_int_va = 0x100,
		messageboxa_iat_va = 0x110,

		// wsprintf()
		wnsprintfa_int_va = 0x120,
		wnsprintfa_iat_va = 0x130,


		// dll name pointers
		kernel32_pointer = 0x150,
		user32_pointer   = 0x160,
		shlwapi_pointer  = 0x170,

		// function name strings (names can spill since they are searched by best starts-with match)
		getstdhandle_va = 0x180,
		writeconsole_va = 0x190,
		exitprocess_va  = 0x1A0,
		messageboxa_va  = 0x1B0,
		wnsprintfa_va   = 0x1C0,

		// dll name strings
		kernel32_va = 0x1D0,
		user32_va   = 0x1E0,
		shlwapi_va  = 0x1F0,
	};
}

#endif 
