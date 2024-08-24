#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include <string>

#include "ErrorReporter.h"

namespace hz
{
	class File
	{
	private:
		std::string _filepath;

	public:
		std::string contents() const;

	public:
		File(std::string&& filepath)
			: _filepath{ std::move(filepath) }
		{
		}
	};
}

#endif 
