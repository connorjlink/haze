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
	inline static constexpr auto CODE_OFFSET = 0x200;
	inline static constexpr auto IMPORTS_OFFSET = 0x400;
	inline static constexpr auto DATA_OFFSET = 0x600;


	inline static constexpr auto IMAGE_SIZE = 0x4000;


	inline static constexpr auto EXIT_PROCESS_VA = 0x402068;
}

#endif 
