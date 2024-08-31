#ifndef HAZE_PEBUILDER_H
#define HAZE_PEBUILDER_H

#include "Constants.h"

#include <string>
#include <vector>
#include <cstdint>

// Haze PEBuilder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class PEBuilder
	{
	// pe utilities
	private:
		byterange make_section(std::string, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);

	private:
		byterange make_import_descriptor(std::uint32_t, std::uint32_t, std::uint32_t);

	private:
		byterange dos_header();
		byterange dos_footer();

	private:
		byterange pe_header();

	private:
		byterange optional_header();

	private:
		// points to the imports VA
		byterange data_directories();

	private:
		byterange sections_table();
		// sections
		byterange imports_section();
		byterange data_section();

	private:
		byterange _code_section;
		byterange _binary;

	public:
		PEBuilder(byterange&& executable)
			: _code_section{ std::move(executable) }, _binary{}
		{
		}

	private:
		void build_pe();

	public:
		void export_exe(const std::string&);
	};
}

#endif 
