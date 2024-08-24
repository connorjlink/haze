#ifndef HAZE_FILEMANAGER_H
#define HAZE_FILEMANAGER_H

#include "File.h"

#include <string>
#include <unordered_map>

namespace hz
{
	class FileManager
	{
	private:
		std::unordered_map<std::string, File> _files;

	public:
		void open_file(const std::string&);
		File& get_file(const std::string&);

	public:
		FileManager()
		{
			_files = {};
		}
	};
}

#endif 
