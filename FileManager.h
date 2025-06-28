#ifndef HAZE_FILEMANAGER_H
#define HAZE_FILEMANAGER_H

#include "File.h"

// Haze FileManager.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class FileManager
	{
	private:
		std::unordered_map<std::string, File> _files;

	public:
		void open_file(const std::string&);
		bool has_file(const std::string&);
		File& get_file(const std::string&);

	public:
		FileManager()
			: _files{}
		{
		}
	};

	extern FileManager* _file_manager;
}

#endif 
