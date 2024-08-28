#ifndef HAZE_BINARYCONSTANTS_H
#define HAZE_BINARYCONSTANTS_H

namespace hz
{
	// for magic
	inline static constexpr auto DOS_HEADER_OFFSET = 0x00;
	// for lfanew
	inline static constexpr auto DOS_FOOTER_OFFSET = 0x30;


	inline static constexpr auto SECTIONS_TABLE_OFFSET = 0x138;
	// sections
	inline static constexpr auto CODE_OFFSET = 0x200;
	inline static constexpr auto IMPORTS_OFFSET = 0x400;
	inline static constexpr auto DATA_OFFSET = 0x600;


	inline static constexpr auto IMAGESIZE = 0x4000;
}

#endif 
