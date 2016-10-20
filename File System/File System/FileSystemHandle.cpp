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
	/* Create a file
	*/
	void FileSystemHandle::createFile(const std::string& file_name, const std::string& data) {
		err::FileError err = _sys.createFile(_dir.getDirectory(), file_name, data);
		if (err::bad(err))
		{
			std::cout << "Error creating file, error: " << err << std::endl;
		}
	}
	/* Get the data in a file
	*/
	void FileSystemHandle::printFile(const std::string& file_name) {
		std::string data;
		err::FileError err = _sys.getFile(_dir.getDirectory(), file_name, data);
		if (err::good(err))
			std::cout << data << std::endl;
		else 
			std::cout << "Error couldn't access file: " << err << std::endl;
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