#include "PEBuilder.h"
#include "X86Emitter.h"
#include "ErrorReporter.h"


namespace
{
	using namespace hz;


	PEBuilder::bytes_t make_section(std::string name, std::uint32_t virtual_size, std::uint32_t virtual_address,
								    std::uint32_t raw_size, std::uint32_t raw_pointer, std::uint32_t characteristics)
	{
		if (name.length() > 8)
		{
			_error_reporter->post_error(std::format("section name `{}` exceeds the maximum allowable length", name), NULL_TOKEN);
		}

		name.resize(8, '\0');

		PEBuilder::bytes_t out{};

		// section name
		for (auto c : name)
		{
			PUT(PEBuilder::make8(c));
		}

		PUT(PEBuilder::make32(virtual_size));
		PUT(PEBuilder::make32(virtual_address));
		PUT(PEBuilder::make32(raw_size));
		PUT(PEBuilder::make32(raw_pointer));

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(PEBuilder::make32(characteristics));

		return out;
	}

	PEBuilder::bytes_t make_import()
	{
		//return 
		return {};
	}



	// at offset 0x00
	PEBuilder::bytes_t dos_magic()
	{
		const auto magic = PEBuilder::make16(0x5A4D);

		PEBuilder::bytes_t out{};

		// at offset 0x00
		PUT(magic);
		PUT(pad16);
		PUT(pad32);
		PUT(pad32);
		PUT(pad32);

		return out;
	}

	// at offset 0x30
	PEBuilder::bytes_t dos_lfanew()
	{
		const auto lfanew = PEBuilder::make32(0x00000040);

		PEBuilder::bytes_t out{};

		PUT(pad32);
		PUT(pad32);
		PUT(pad32);
		PUT(lfanew);

		return out;
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

	// at offset 0x138
	PEBuilder::bytes_t sections_table()
	{
		return std::vector<std::uint8_t>
		{
															  0x2E, 0x74, 0x65, 0x78,  0x74, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x00, 0x00,  0x00, 0x10, 0x00, 0x00,  0x00, 0x02, 0x00, 0x00,  0x00, 0x02, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x20, 0x00, 0x00, 0x60,
			0x2E, 0x72, 0x64, 0x61,  0x74, 0x61, 0x00, 0x00,  0x00, 0x10, 0x00, 0x00,  0x00, 0x20, 0x00, 0x00,
			0x00, 0x02, 0x00, 0x00,  0x00, 0x04, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,  0x40, 0x00, 0x00, 0x40,  0x2E, 0x64, 0x61, 0x74,  0x61, 0x00, 0x00, 0x00,
			0x00, 0x10, 0x00, 0x00,  0x00, 0x30, 0x00, 0x00,  0x00, 0x02, 0x00, 0x00,  0x00, 0x06, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x40, 0x00, 0x00, 0xC0,
		};
	}

	// at offset 0x200
	PEBuilder::bytes_t code_section()
	{
		PEBuilder::bytes_t out{};

		//auto emitter = new X86Emitter{ {}, "test.exe" };
		//PUT(emitter->emit_copy((Register)X86Emitter::EAX, 5));
		//PUT(emitter->emit_copy((Register)X86Emitter::ECX, 2));
		//PUT(emitter->emit_iadd((Register)X86Emitter::EAX, (Register)X86Emitter::ECX));
		PUT(PEBuilder::make8(0xB8)); // mov eax, 0x5
		PUT(PEBuilder::make32(0x00000005));

		PUT(PEBuilder::make8(0xB9)); // mov ecx, 0x2
		PUT(PEBuilder::make32(0x00000002));

		PUT(PEBuilder::make8(0x01)); // add eax, ecx
		PUT(PEBuilder::make8(0xC8));

		PUT(PEBuilder::make8(0x50)); // push eax
		//
		// PUT(PEBuilder::make16(0x00))
		//PUT(PEBuilder::make16(0x036A));
		PUT(PEBuilder::make8(0xFF)); // call DWORD PTR ds:0x402068
		PUT(PEBuilder::make8(0x15)); 
		PUT(PEBuilder::make8(0x68));
		PUT(PEBuilder::make8(0x20));
		PUT(PEBuilder::make8(0x40));
		PUT(PEBuilder::make8(0x00));

		return out;
		//return std::vector<std::uint8_t>
		//{
		//	0x6A, 0x00, 0x68, 0x00, 0x30, 0x40, 0x00, 0x68, 0x17, 0x30, 0x40, 0x00, 0x6A, 0x00, 0xFF, 0x15,
		//	0x70, 0x20, 0x40, 0x00, 0x6A, 0x00, 0xFF, 0x15, 0x68, 0x20, 0x40, 0x00,
		//	//0x6A, 0x05, 0xFF, 0x15,  0x68, 0x20, 0x40, 0x00,
		//};
	}

	// at offset 0x400
	PEBuilder::bytes_t imports_section()
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

	// at offset 0x600
	PEBuilder::bytes_t data_section()
	{
		return std::vector<std::uint8_t>
		{
			0x61, 0x20, 0x73, 0x69, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x50, 0x45, 0x20, 0x65, 0x78, 0x65, 0x63,
			0x75, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72,
			0x6C, 0x64, 0x21, 0x00,
		};
	}

}

namespace hz
{
	PEBuilder::bytes_t PEBuilder::make8(std::uint8_t value)
	{
		const byte b0 = value;
		return bytes_t{ b0 };
	}

	PEBuilder::bytes_t PEBuilder::make16(std::uint16_t value)
	{
		const byte b0 = (value & 0xFF) >> 0;
		const byte b1 = (value & 0xFF00) >> 8;
		return bytes_t{ b0, b1 };
	}

	PEBuilder::bytes_t PEBuilder::make32(std::uint32_t value)
	{
		const byte b0 = (value & 0xFF) >> 0;
		const byte b1 = (value & 0xFF00) >> 8;
		const byte b2 = (value & 0xFF0000) >> 16;
		const byte b3 = (value & 0xFF000000) >> 24;
		return bytes_t{ b0, b1, b2, b3 };
	}


	PEBuilder::bytes_t PEBuilder::build()
	{
		PEBuilder::bytes_t binary{};
		binary.resize(0x4000); // image size

		auto dos = dos_magic();
		auto footer = dos_lfanew();
		auto pe = pe_header();
		auto optional = optional_header();

		auto directories = data_directories();
		auto sections = sections_table();

		auto code = code_section();
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