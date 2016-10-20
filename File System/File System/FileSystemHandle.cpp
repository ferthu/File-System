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
	/* Remove a file
	*/
	void FileSystemHandle::removeFile(const std::string& file_name)
	{
		err::FileError err = _sys.removeFile(_dir.getDirectory(), file_name);
		if (err::bad(err))
		{
			std::cout << "Error removing file, error: " << err << std::endl;
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
	
	void FileSystemHandle::copyFile(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName)
	{
		DirectoryReference source;
		DirectoryReference target;

		source.directoryFromString(_dir, sourceDirectory, _sys);
		target.directoryFromString(_dir, targetDirectory, _sys);

		std::string fileContents;
		err::FileError err = _sys.getFile(source.getDirectory(), sourceFileName, fileContents);
		if (err::good(err))
		{
			err = _sys.createFile(target.getDirectory(), targetFileName, fileContents);
			if (err::bad(err))
			{
				std::cout << "Error copying file: " << err << std::endl;
			}
		}
		else
		{
			std::cout << "Error reading file: " << err << std::endl;
		}
	}

	void FileSystemHandle::appendFile(const std::string& toAppendDirectory, const std::string& toAppendFileName, const std::string& appendDataDirectory, const std::string& appendDataFileName)
	{
		DirectoryReference toAppend;
		DirectoryReference appendData;

		toAppend.directoryFromString(_dir, toAppendDirectory, _sys);
		appendData.directoryFromString(_dir, appendDataDirectory, _sys);

		std::string originalContents;
		std::string appendContents;

		// read original file
		err::FileError err = _sys.getFile(toAppend.getDirectory(), toAppendFileName, originalContents);
		if (err::good(err))
		{
			// read append data
			err::FileError err = _sys.getFile(appendData.getDirectory(), appendDataFileName, appendContents);
			if (err::good(err))
			{
				// remove original file
				err = _sys.removeFile(toAppend.getDirectory(), toAppendFileName);
				if (err::good(err))
				{
					// create new appended file
					std::string newContents = originalContents + appendContents;
					err = _sys.createFile(toAppend.getDirectory(), toAppendFileName, newContents);
					if (err::bad(err))
						std::cout << "Error creating appended file, error: " << err << std::endl;
				}
				else
					std::cout << "Error replacing file: " << err << std::endl;
			}
			else
				std::cout << "Error reading second file: " << err << std::endl;
		}
		else
			std::cout << "Error reading first file: " << err << std::endl;

	}

	void FileSystemHandle::moveFile(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName)
	{
		copyFile(sourceDirectory, sourceFileName, targetDirectory, targetFileName);
		
		DirectoryReference sourceDir;
		sourceDir.directoryFromString(_dir, sourceDirectory, _sys);

		err::FileError err = _sys.removeFile(sourceDir.getDirectory(), sourceFileName);
		if (err::bad(err))
		{
			std::cout << "Error removing file, error: " << err << std::endl;
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
			_dir.directoryFromString(_dir, str, _sys);
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

	void FileSystemHandle::format()
	{
		_dir.format();
		_sys.format();
	}

	std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr) {
		os << dr._dir;
		return os;
	}
}