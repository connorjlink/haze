#ifndef HAZE_RISCVWIDTHTYPE_H
#define HAZE_RISCVWIDTHTYPE_H

// Haze RISCVWidthType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVWidthType
	{
		BYTE, HALFWORD, WORD,
	};

	static const std::unordered_map<RISCVWidthType, std::string> _width_type_map
	{
		{ RISCVWidthType::BYTE, "byte" },
		{ RISCVWidthType::HALFWORD, "halfword" },
		{ RISCVWidthType::WORD, "word" },
	};
}

#endif
