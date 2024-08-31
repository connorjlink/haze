#ifndef HAZE_FILEMANAGER_H
#define HAZE_FILEMANAGER_H

#include "File.h"

#include <string>
#include <unordered_map>

// Haze FileManager.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class FileManager
	{
	private:
		std::unordered_map<std::string, File> _files;

	public:
		std::string _current_file;

	public:
		void open_file(const std::string&);
		File& get_file(const std::string&);

	public:
		FileManager()
			: _files{}, _current_file{ "" }
		{
		}
	};

	extern FileManager* _file_manager;
}

#endif 
