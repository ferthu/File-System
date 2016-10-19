#include "FileSystemHandle.h"

namespace file {

	FileSystemHandle::FileSystemHandle()
	{
	}


	FileSystemHandle::~FileSystemHandle()
	{
	}

	void FileSystemHandle::createFolder(const std::string& name) {
		err::FileError err = _sys.createFolder(_dir.getDirectory(), name);

		if (err::bad(err))
		{
			std::cout << "Error creating folder, error: " << err << std::endl;
		}
	}
	void FileSystemHandle::listDirectory() {
		std::vector<std::string> list;
		err::FileError err = _sys.listDir(_dir.getDirectory(), list);

		if (err::good(err))
		{
			std::cout << "Listing directory:" << std::endl;

			int size = list.size();

			for (int i = 0; i < size; i++)
			{
				std::cout << list[i] << std::endl;
			}
		}
		else
		{
			std::cout << "Error listing files, error: " << err << std::endl;
		}
	}
	void FileSystemHandle::cd(const std::string& str) {
		try
		{
			_dir.directoryFromString(str, _sys);
		}
		catch (err::FileError e)
		{
			std::cout << "File error: " << e << std::endl;
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << e.what();
		}
	}
	std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr) {
		os << dr._dir;
		return os;
	}
}