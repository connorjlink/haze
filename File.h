#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include <string>

#include "ToolchainType.h"

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
