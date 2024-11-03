#ifndef HAZE_INTTYPETYPE_H
#define HAZE_INTTYPETYPE_H

// Haze IntTypeType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class IntTypeType
	{
		INT8, // byte 
		INT16, // word 
		INT32, // dword
		INT64, // qword
	};

	static const std::unordered_map<IntTypeType, std::string> _int_type_type_map
	{
		{ IntTypeType::INT8, "byte" },
		{ IntTypeType::INT16, "word" },
		{ IntTypeType::INT32, "double word" },
		{ IntTypeType::INT64, "quad word" },
	};
}

#endif
