#include "PEBuilder.h"
#include "X86Emitter.h"
#include "ErrorReporter.h"


namespace
{
	using namespace hz;


	

	PEBuilder::bytes_t make_import()
	{
		//return 

		return {};
	}



	

	

	// NOTE: pe and optional header immediately adjacent in the final image
	// at offset 0x40
	PEBuilder::bytes_t pe_header()
	{
		const auto signature = PEBuilder::make32(0x00004550);
		const auto machine = PEBuilder::make16(0x014C);
		const auto section_count = PEBuilder::make16(0x0003);
		const auto optional_size = PEBuilder::make16(0x00E0);
		const auto characteristics = PEBuilder::make16(0x0102);

		PEBuilder::bytes_t out{};

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

	// at offset 0x58
	PEBuilder::bytes_t optional_header()
	{
		const auto magic = PEBuilder::make16(0x010B);
		const auto entrypoint = PEBuilder::make32(0x00001000);
		const auto image_base = PEBuilder::make32(0x00400000);
		const auto section_alignment = PEBuilder::make32(0x00001000);
		const auto file_alignment = PEBuilder::make32(0x00000200);
		const auto major_subsystem = PEBuilder::make16(0x0004);
		// `image size` - need to modify when making changes to the underlying object code
		const auto image_size = PEBuilder::make32(0x00004000);
		const auto header_size = PEBuilder::make32(0x00000200);
		//const auto subsystem = PEBuilder::make16(0x0002); // for GUI?
		const auto subsystem = PEBuilder::make16(0x0003); // for CLI?
		const auto directory_count = PEBuilder::make32(0x00000010);

		PEBuilder::bytes_t out{};

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

	// TODO: at offset 0xB8?
	PEBuilder::bytes_t data_directories()
	{
		const auto imports_va = PEBuilder::make32(0x00002000);

		PEBuilder::bytes_t out{};

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

	
}

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

		// Haze Optimizing Compiler Executable
		// (c) 2024 Connor J. Link. All Rights Reserved.

		return std::vector<std::uint8_t>
		{
			0x61, 0x20, 0x73, 0x69, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x50, 0x45, 0x20, 0x65, 0x78, 0x65, 0x63,
			0x75, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72,
			0x6C, 0x64, 0x21, 0x00,
		};
	}

	


	PEBuilder::bytes_t PEBuilder::build(bytes_t code)
	{
		PEBuilder::bytes_t binary{};
		binary.resize(0x4000); // image size

		auto dos = dos_magic();
		auto footer = dos_lfanew();
		auto pe = pe_header();
		auto optional = optional_header();

		auto directories = data_directories();
		auto sections = sections_table();

		//auto code = code_section();
		auto imports = imports_section();
		auto data = data_section();

		auto head = binary.begin();

		std::copy(dos.begin(), dos.end(), head + 0x00);
		std::copy(footer.begin(), footer.end(), head + 0x30);
		std::copy(pe.begin(), pe.end(), head + 0x40);

		optional.append_range(directories);

		//std::copy(directories.begin(), directories.end(), optional.end());
		std::copy(optional.begin(), optional.end(), head + 0x58);
		//std::copy(directories.begin(), directories.end(), head + ? );
		std::copy(sections.begin(), sections.end(), head + 0x138);

		std::copy(code.begin(), code.end(), head + 0x200);
		std::copy(imports.begin(), imports.end(), head + 0x400);
		std::copy(data.begin(), data.end(), head + 0x600);

		return binary;
	}
}