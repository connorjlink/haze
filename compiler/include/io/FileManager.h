#ifndef HAZE_FILEMANAGER_H
#define HAZE_FILEMANAGER_H

#include <io/models/File.h>

// Haze FileManager.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct FileManager : 
		public SingletonTag<FileManager>,
		public InjectSingleton<ErrorReporter>
	{
	private:
		std::unordered_map<std::filesystem::path, File> files;

	public:
		// will overwrite the file entry if it already exists
		void open_file(const std::filesystem::path&);
		bool has_file(const std::filesystem::path&);
		File& get_file(const std::filesystem::path&);
		void update_file(const std::filesystem::path&, std::string);

	private:
		[[noreturn]]
		void never_opened_error(const std::filesystem::path&) const;

	public:
		FileManager()
			: files{}
		{
		}
	};
}

#endif 
