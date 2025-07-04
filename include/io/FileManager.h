#ifndef HAZE_FILEMANAGER_H
#define HAZE_FILEMANAGER_H

#include "File.h"
#include "DependencyInjector.h"
#include "ErrorReporter.h"

// Haze FileManager.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class FileManager : 
		public SingletonTag<FileManager>,
		public InjectSingleton<ErrorReporter>
	{
	private:
		std::unordered_map<std::string, File> _files;

	public:
		// will overwrite the file entry if it already exists
		void open_file(const std::string&);
		bool has_file(const std::string&);
		File& get_file(const std::string&);
		void update_file(const std::string&, const std::string&);

	private:
		[[noreturn]]
		void never_opened_error(const std::string&) const;

	public:
		FileManager()
			: _files{}
		{
		}
	};
}

#endif 
