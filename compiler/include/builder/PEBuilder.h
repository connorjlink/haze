#ifndef HAZE_PEBUILDER_H
#define HAZE_PEBUILDER_H

#include <builder/Builder.h>
#include <utility/Constants.h>

// Haze PEBuilder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class PEBuilder : public Builder
	{
	// pe utilities
	private:
		ByteRange make_section(std::string, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);

	private:
		ByteRange make_import_descriptor(std::uint32_t, std::uint32_t, std::uint32_t);

	private:
		ByteRange dos_header();
		ByteRange dos_footer();

	private:
		ByteRange pe_header();

	private:
		ByteRange optional_header();

	private:
		// points to the imports VA
		ByteRange data_directories();

	private:
		ByteRange sections_table();
		// sections
		ByteRange imports_section();
		ByteRange data_section();

	private:
		void build_pe();

	public:
		virtual void export_exe(const std::string&) override;
	};
}

#endif 
