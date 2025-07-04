#ifndef HAZE_PEBUILDER_H
#define HAZE_PEBUILDER_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Constants.h>

// Haze PEBuilder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class PEBuilder
		: public InjectSingleton<ErrorReporter>
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
		const byterange& _code_section;
		byterange _binary;

	public:
		PEBuilder(const byterange& executable)
			: _code_section{ executable }, _binary{}
		{
		}

	private:
		void build_pe();

	public:
		void export_exe(const std::string&);
	};
}

#endif 
