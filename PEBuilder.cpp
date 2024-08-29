#include "PEBuilder.h"
#include "X86Emitter.h"
#include "BinaryUtilities.h"
#include "ErrorReporter.h"
#include "BinaryConstants.h"

#include <fstream>

// Haze PEBuilder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	byterange PEBuilder::make_section(std::string name, std::uint32_t virtual_size, std::uint32_t virtual_address,
		std::uint32_t raw_size, std::uint32_t raw_pointer, std::uint32_t characteristics)
	{
		if (name.length() > 8)
		{
			_error_reporter->post_error(std::format("section name `{}` exceeds the maximum allowable length", name), NULL_TOKEN);
		}

		name.resize(8, '\0');

		byterange out{};

		// insert section name char by char
		for (auto c : name)
		{
			PUT(BinaryUtilities::range8(c));
		}

		PUT(BinaryUtilities::range32(virtual_size));
		PUT(BinaryUtilities::range32(virtual_address));
		PUT(BinaryUtilities::range32(raw_size));
		PUT(BinaryUtilities::range32(raw_pointer));

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(BinaryUtilities::range32(characteristics));

		return out;
	}

	byterange PEBuilder::make_import()
	{
		//return 

		return {};
	}


	byterange PEBuilder::dos_header()
	{
		const auto magic = BinaryUtilities::range16(0x5A4D);

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
		const auto lfanew = BinaryUtilities::range32(0x00000040);

		byterange out{};

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(lfanew);

		return out;
	}


	byterange PEBuilder::pe_header()
	{
		const auto signature = BinaryUtilities::range32(0x00004550);
		const auto machine = BinaryUtilities::range16(0x014C);
		const auto section_count = BinaryUtilities::range16(0x0003);
		const auto optional_size = BinaryUtilities::range16(0x00E0);
		const auto characteristics = BinaryUtilities::range16(0x0102);

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
		const auto magic = BinaryUtilities::range16(0x010B);
		const auto entrypoint = BinaryUtilities::range32(0x00001000);
		const auto image_base = BinaryUtilities::range32(0x00400000);
		// VAs will start at 0x00401000
		const auto section_alignment = BinaryUtilities::range32(0x00001000);
		const auto file_alignment = BinaryUtilities::range32(0x00000200);
		const auto major_subsystem = BinaryUtilities::range16(0x0004);
		// `image size` - need to modify when making changes to the underlying object code
		const auto image_size = BinaryUtilities::range32(0x00004000); // 16k
		const auto header_size = BinaryUtilities::range32(0x00000200);
		//const auto subsystem = PEBuilder::make16(0x0002); // if using GUI
		const auto subsystem = BinaryUtilities::range16(0x0003); //  /SUBSYSTEM:CONSOLE
		const auto directory_count = BinaryUtilities::range32(0x00000010); // max 16

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
		const auto imports_va = BinaryUtilities::range32(0x00002000);

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
		// TODO: figure out spacing on this function 
		return std::vector<std::uint8_t>
		{
			                                                0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60,
			0x2E, 0x72, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
			0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xC0,
		};
	}


	byterange PEBuilder::imports_section()
	{
		return std::vector<std::uint8_t>
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
		};
	}

	byterange PEBuilder::data_section()
	{
		// for any compile-time strings to be used 

		// Current String:

		// Haze Optimizing Compiler Executable
		// (c) Connor J. Link. All Rights Reserved.

		return std::vector<std::uint8_t>
		{
			0x48, 0x61, 0x7A, 0x65,  0x20, 0x4F, 0x70, 0x74,  0x69, 0x6D, 0x69, 0x7A,  0x69, 0x6E, 0x67, 0x20, 
			0x43, 0x6F, 0x6D, 0x70,  0x69, 0x6C, 0x65, 0x72,  0x20, 0x45, 0x78, 0x65,  0x63, 0x75, 0x74, 0x61, 
			0x62, 0x6C, 0x65, 0x0A,  0x28, 0x63, 0x29, 0x20,  0x43, 0x6F, 0x6E, 0x6E,  0x6F, 0x72, 0x20, 0x4A, 
			0x2E, 0x20, 0x4C, 0x69,  0x6E, 0x6B, 0x2E, 0x20,  0x41, 0x6C, 0x6C, 0x20,  0x52, 0x69, 0x67, 0x68, 
			0x74, 0x73, 0x20, 0x52,  0x65, 0x73, 0x65, 0x72,  0x76, 0x65, 0x64, 0x2E,
		};
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