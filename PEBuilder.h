#ifndef HAZE_PEBUILDER_H
#define HAZE_PEBUILDER_H

#include <string>
#include <vector>
#include <cstdint>

#include "Constants.h"
#include "BinaryUtilities.h"

#define PUT(x) out.append_range(x)

namespace hz
{
	class PEBuilder
	{
	// pe utilities
	private:
		byterange make_section(std::string, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
		byterange make_import();


	private:
		byterange dos_header();
		byterange dos_footer();

	private:
		byterange pe_header();

	private:
		byterange sections_table();
		// sections
		byterange imports_section();
		byterange data_section();

	public:
		// code is at offset 0x200
		void write_code(byterange);

	public:
		void build_pe();
		void export_exe();
	};
}

#endif 
