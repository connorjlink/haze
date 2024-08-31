#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include "ToolchainType.h"

#include <string>

// Haze File.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class File
	{
	private:
		std::string _filepath;
		ToolchainType _type;

	public:
		std::string contents();
		ToolchainType ttype() const;

	public:
		void compute_type();

	public:
		File() = default;

		File(const std::string& filepath)
			: _filepath{ filepath }, _type{}
		{
		}
	};
}

#endif 
