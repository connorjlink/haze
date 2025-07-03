import std;

#include "PEBuilder.h"
#include "X86Emitter.h"
#include "BinaryUtilities.h"
#include "BinaryConstants.h"
#include "JobManager.h"
#include "ErrorReporter.h"

// Haze PEBuilder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	byterange PEBuilder::make_section(std::string name, std::uint32_t virtual_size, std::uint32_t virtual_address,
								      std::uint32_t raw_size, std::uint32_t raw_pointer, std::uint32_t characteristics)
	{
		if (name.length() > 7)
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("section name `{}` exceeds the maximum allowable length", name), NULL_TOKEN);
		}

		name.resize(7, '\0');
		// if the string was too big, then make sure there is a terminator byte still

		byterange out{};

		// insert section name char by char
		PUT(bin::range_string(name));

		PUT(bin::range32(virtual_size));
		PUT(bin::range32(virtual_address));
		PUT(bin::range32(raw_size));
		PUT(bin::range32(raw_pointer));

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(bin::range32(characteristics));

		return out;
	}


	byterange PEBuilder::make_import_descriptor(std::uint32_t int_pointer, std::uint32_t dll_pointer, std::uint32_t iat_pointer)
	{
		byterange out{};

		PUT(bin::range32(int_pointer));
		PUT(pad32);
		PUT(pad32);
		PUT(bin::range32(dll_pointer));
		PUT(bin::range32(iat_pointer));

		return out;
	}


	byterange PEBuilder::dos_header()
	{
		const auto magic = bin::range16(0x5A4D);

		byterange out{};

		PUT(magic);
		PUT(pad16);
		PUT(pad32);
		PUT(pad32);
		PUT(pad32);

		return out;
	}

	byterange PEBuilder::dos_footer()
	{
		const auto lfanew = bin::range32(0x00000040);

		byterange out{};

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(lfanew);

		return out;
	}


	byterange PEBuilder::pe_header()
	{
		const auto signature = bin::range32(0x00004550);
		const auto machine = bin::range16(0x014C);
		const auto section_count = bin::range16(0x0003);
		const auto optional_size = bin::range16(0x00E0);
		const auto characteristics = bin::range16(0x0102);

		byterange out{};

		PUT(signature);
		PUT(machine);
		PUT(section_count);
		PUT(pad32);
		PUT(pad32);

		PUT(pad32);
		PUT(optional_size);
		PUT(characteristics);

		return out;
	}


	byterange PEBuilder::optional_header()
	{
		const auto magic = bin::range16(0x010B);
		const auto entrypoint = bin::range32(0x00001000);
		const auto image_base = bin::range32(0x00400000);
		// VAs will start at 0x00401000
		const auto section_alignment = bin::range32(0x00001000);
		const auto file_alignment = bin::range32(0x00000200);
		const auto major_subsystem = bin::range16(0x0004);
		// `image size` - need to modify when making changes to the underlying object code
		const auto image_size = bin::range32(0x00004000); // 16k
		const auto header_size = bin::range32(0x00000200);
		//const auto subsystem = PEBuilder::make16(0x0002); // if using GUI
		const auto subsystem = bin::range16(0x0003); //  /SUBSYSTEM:CONSOLE
		const auto directory_count = bin::range32(0x00000010); // max 16

#pragma message("TODO: support growing the stack here to make overflowing at fixed 16k frame size harder!")

		byterange out{};

		PUT(magic);
		PUT(pad16);
		PUT(pad32);

		PUT(pad32);
		PUT(pad32);
		PUT(entrypoint);
		PUT(pad32);

		PUT(pad32);
		PUT(image_base);
		PUT(section_alignment);
		PUT(file_alignment);

		PUT(pad32);
		PUT(pad32);
		PUT(major_subsystem);
		PUT(pad16);
		PUT(pad32);

		PUT(image_size);
		PUT(header_size);
		PUT(pad32);
		PUT(subsystem);
		PUT(pad16);

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(pad32);

		PUT(pad32);
		PUT(directory_count);

		return out;
	}


	byterange PEBuilder::data_directories()
	{
		const auto imports_va = bin::range32(0x00002000);

		byterange out{};

		PUT(pad32);
		PUT(pad32);

		PUT(imports_va);
		PUT(pad32);
		PUT(pad32);
		PUT(pad32);

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(pad32);

		return out;
	}


	byterange PEBuilder::sections_table()
	{
		/*return std::vector<std::uint8_t>
		{
			                                                0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60,
			0x2E, 0x72, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
			0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xC0,
		};*/

		byterange out{};

		PUT(make_section(".text", 0x1000, 0x1000, 0x200, CODE_OFFSET, 0x60000020)); // code execute read
		PUT(make_section(".rdata", 0x1000, 0x2000, 0x200, IMPORTS_OFFSET, 0x40000040)); // initialized read
		PUT(make_section(".data", 0x1000, 0x3000, 0x200, DATA_OFFSET, 0xC0000040)); // data read write

		return out;
	}


	byterange PEBuilder::imports_section()
	{
		/*return std::vector<std::uint8_t>
		{
			0x3C, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x20, 0x00, 0x00,
			0x68, 0x20, 0x00, 0x00, 0x44, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x85, 0x20, 0x00, 0x00, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x20, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x78,
			0x69, 0x74, 0x50, 0x72, 0x6F, 0x63, 0x65, 0x73, 0x73, 0x00, 0x00, 0x00, 0x4D, 0x65, 0x73, 0x73,
			0x61, 0x67, 0x65, 0x42, 0x6F, 0x78, 0x41, 0x00, 0x4C, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B, 0x65, 0x72, 0x6E, 0x65, 0x6C, 0x33, 0x32,
			0x2E, 0x64, 0x6C, 0x6C, 0x00, 0x75, 0x73, 0x65, 0x72, 0x33, 0x32, 0x2E, 0x64, 0x6C, 0x6C, 0x00,
		};*/

		byterange out{};
		out.resize(0x200);

		auto head = out.begin();

		const auto base = 0x2000; // RVA - base_address

#define CALC(x) (base + x)

		const auto dummy_descriptor = make_import_descriptor(0x0, 0x0, 0x0);
		const auto length = dummy_descriptor.size();

		const auto getstdhandle_descriptor = make_import_descriptor(CALC(getstdhandle_int_va), CALC(kernel32_va), CALC(getstdhandle_iat_va));
		const auto writeconsole_descriptor = make_import_descriptor(CALC(writeconsole_int_va), CALC(kernel32_va), CALC(writeconsole_iat_va));
		const auto exitprocess_descriptor = make_import_descriptor(CALC(exitprocess_int_va), CALC(kernel32_va), CALC(exitprocess_iat_va));
		const auto messageboxa_descriptor = make_import_descriptor(CALC(messageboxa_int_va), CALC(user32_va), CALC(messageboxa_iat_va));
		const auto wnsprintfa_descriptor = make_import_descriptor(CALC(wnsprintfa_int_va), CALC(shlwapi_va), CALC(wnsprintfa_iat_va));


		std::copy(getstdhandle_descriptor.begin(), getstdhandle_descriptor.end(), head + (length * 0));
		std::copy(writeconsole_descriptor.begin(), writeconsole_descriptor.end(), head + (length * 1));
		std::copy(exitprocess_descriptor.begin(), exitprocess_descriptor.end(), head + (length * 2));
		std::copy(messageboxa_descriptor.begin(), messageboxa_descriptor.end(), head + (length * 3));
		std::copy(wnsprintfa_descriptor.begin(), wnsprintfa_descriptor.end(), head + (length * 4));
		std::copy(dummy_descriptor.begin(), dummy_descriptor.end(), head + (length * 5));



		auto getstdhandle_int_iat = bin::range32(base + getstdhandle_va);
		getstdhandle_int_iat.append_range(bin::range32(0x00000000));
		std::copy(getstdhandle_int_iat.begin(), getstdhandle_int_iat.end(), head + getstdhandle_int_va);
		std::copy(getstdhandle_int_iat.begin(), getstdhandle_int_iat.end(), head + getstdhandle_iat_va);

		auto writeconsole_int_iat = bin::range32(base + writeconsole_va);
		writeconsole_int_iat.append_range(bin::range32(0x00000000));
		std::copy(writeconsole_int_iat.begin(), writeconsole_int_iat.end(), head + writeconsole_int_va);
		std::copy(writeconsole_int_iat.begin(), writeconsole_int_iat.end(), head + writeconsole_iat_va);

		auto exitprocess_int_iat = bin::range32(base + exitprocess_va);
		exitprocess_int_iat.append_range(bin::range32(0x00000000));
		std::copy(exitprocess_int_iat.begin(), exitprocess_int_iat.end(), head + exitprocess_int_va);
		std::copy(exitprocess_int_iat.begin(), exitprocess_int_iat.end(), head + exitprocess_iat_va);

		auto messageboxa_int_iat = bin::range32(base + messageboxa_va);
		messageboxa_int_iat.append_range(bin::range32(0x00000000));
		std::copy(messageboxa_int_iat.begin(), messageboxa_int_iat.end(), head + messageboxa_int_va);
		std::copy(messageboxa_int_iat.begin(), messageboxa_int_iat.end(), head + messageboxa_iat_va);

		auto wnsprintfa_int_iat = bin::range32(base + wnsprintfa_va);
		wnsprintfa_int_iat.append_range(bin::range32(0x00000000));
		std::copy(wnsprintfa_int_iat.begin(), wnsprintfa_int_iat.end(), head + wnsprintfa_int_va);
		std::copy(wnsprintfa_int_iat.begin(), wnsprintfa_int_iat.end(), head + wnsprintfa_iat_va);

		// NOTE: format is hint, name
		// ignoring hint addresses and using only by-name imports for now

		// function name imports
		auto getstdhandle = bin::range16(0x0000);
		getstdhandle.append_range(bin::range_string("GetStdHandle")); // from kernel32.dll
		std::copy(getstdhandle.begin(), getstdhandle.end(), head + getstdhandle_va);

		auto writeconsole = bin::range16(0x0000);
		writeconsole.append_range(bin::range_string("WriteConsoleA")); // from kernel32.dll
		std::copy(writeconsole.begin(), writeconsole.end(), head + writeconsole_va);

		auto exitprocess = bin::range16(0x0000);
		exitprocess.append_range(bin::range_string("ExitProcess")); // from kernel32.dll
		std::copy(exitprocess.begin(), exitprocess.end(), head + exitprocess_va);

		auto messageboxa = bin::range16(0x0000);
		messageboxa.append_range(bin::range_string("MessageBoxA")); // from user32.dll
		std::copy(messageboxa.begin(), messageboxa.end(), head + messageboxa_va);

		auto wnsprintfa = bin::range16(0x0000);
		wnsprintfa.append_range(bin::range_string("wnsprintfA")); // from shlwapi.dll
		std::copy(wnsprintfa.begin(), wnsprintfa.end(), head + wnsprintfa_va);
		
		
		// dll name imports
		const auto kernel32 = bin::range_string("kernel32.dll");
		std::copy(kernel32.begin(), kernel32.end(), head + kernel32_va);

		const auto user32 = bin::range_string("user32.dll");
		std::copy(user32.begin(), user32.end(), head + user32_va);

		const auto shlwapi = bin::range_string("shlwapi.dll");
		std::copy(shlwapi.begin(), shlwapi.end(), head + shlwapi_va);


		return out;
	}

	// compile-time ROM strings section
	byterange PEBuilder::data_section()
	{
		byterange out{};

		const auto logo_string = bin::range_string("Haze Optimizing Compiler Executable\n(c) Connor J. Link. All Rights Reserved.\n");
		PUT(logo_string);

		const auto output_string = bin::range_string("Program output: ");
		PUT(output_string);

		const auto format_string = bin::range_string("%d\n");
		PUT(format_string);

		return out;
	}


	void PEBuilder::build_pe()
	{
		// NOTE: extremely minimal PE builder
		// Any and all optional information is abbreviated
		
		_binary.resize(IMAGE_SIZE);
		auto head = _binary.begin();

		// legacy DOS header
		const auto header = dos_header();
		const auto footer = dos_footer();

		// modern PE header
		const auto pe = pe_header();
		const auto optional = optional_header();

		// pointer metadata
		const auto directories = data_directories();
		const auto sections = sections_table();

		// program data
		const auto code = _code_section;
		const auto imports = imports_section();
		const auto data = data_section();

		
		// NOTE: not using std::move() here since profiling shows its slower here

		std::copy(header.begin(), header.end(), head + DOS_HEADER_OFFSET);
		std::copy(footer.begin(), footer.end(), head + DOS_FOOTER_OFFSET);

		std::copy(pe.begin(), pe.end(), head + PE_HEADER_OFFSET);
		std::copy(optional.begin(), optional.end(), head + OPTIONAL_HEADER_OFFSET);

		std::copy(directories.begin(), directories.end(), head + DATA_DIRECTORIES_OFFSET);
		std::copy(sections.begin(), sections.end(), head + SECTIONS_TABLE_OFFSET);

		std::copy(code.begin(), code.end(), head + CODE_OFFSET);
		std::copy(imports.begin(), imports.end(), head + IMPORTS_OFFSET);
		std::copy(data.begin(), data.end(), head + DATA_OFFSET);

	}

	void PEBuilder::export_exe(const std::string& filepath)
	{
		build_pe();

		auto binfile = std::fstream(filepath, std::ios::binary | std::ios::out);
		binfile.write(reinterpret_cast<const char*>(_binary.data()), _binary.size());
	}
}