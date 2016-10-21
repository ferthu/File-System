#include "FileSystemHandle.h"
#include"FileError.h"

namespace file {

	FileSystemHandle::FileSystemHandle()
	{
		_sys = std::make_unique<FileSystem>();
	}


	FileSystemHandle::~FileSystemHandle()
	{
	}

	void FileSystemHandle::cd(const std::string& str) {
		constructDirRef(str, _dir);
	}
	bool FileSystemHandle::constructDirRef(const std::string& str, DirectoryReference& ref) {
		try
		{
			_dir.directoryFromString(_dir, str, *_sys);
		}
		catch (err::FileError e)
		{
			std::cout << "File error: " << e << std::endl;
			return true;
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << e.what();
			return true;
		}
		return false;
	}

	void FileSystemHandle::createFolder(const std::string& name) {
		err::FileError error = _sys->createFolder(_dir.getDirectory(), name);

		if (err::bad(error))
		{
			std::cout << "Error creating folder, error: " << err::getMsg(error) << std::endl;
		}
	}
	/* Create a file
	*/
	void FileSystemHandle::createFile(const std::string& file_name, char access, const std::string& data) {
		err::FileError error = _sys->createFile(_dir.getDirectory(), file_name, access, data);
		if (err::bad(error))
		{
			std::cout << "Error creating file, error: " << err::getMsg(error) << std::endl;
		}
	}
	/* Remove a file or directory
	*/
	void FileSystemHandle::remove(const std::string& path)
	{
		err::FileError error = _sys->remove(_dir.getDirectory(), path);
		if (err::bad(error))
		{
			std::cout << "Error removing item, error: " << err::getMsg(error) << std::endl;
		}
	}
	/* Get the data in a file
	*/
	void FileSystemHandle::printFile(const std::string& file_name) {
		std::string data;
		err::FileError error = _sys->getFile(_dir.getDirectory(), file_name, data);
		if (err::good(error))
			std::cout << data << std::endl;
		else 
			std::cout << "Couldn't access file, error: " << err::getMsg(error) << std::endl;
	}
	
	void FileSystemHandle::copyFile(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName)
	{
		DirectoryReference source, target;

		//Construct directories.
		if (constructDirRef(sourceDirectory, source) ||
			constructDirRef(targetDirectory, target))
			//Bad file path
			return;

		std::string fileContents;
		err::FileError err = _sys->copy(source.getDirectory(), sourceFileName, target.getDirectory(), targetFileName);
		if (err::bad(err))
		{
			std::cout << "Error reading file: " << err::getMsg(err) << std::endl;
		}
	}

	void FileSystemHandle::appendFile(const std::string& toAppendDirectory, const std::string& toAppendFileName, const std::string& appendDataDirectory, const std::string& appendDataFileName)
	{
		DirectoryReference toAppend, appendData;
		//Construct directories.
		if (constructDirRef(toAppendDirectory, toAppend) ||
			constructDirRef(appendDataDirectory, appendData))
			//Bad file path
			return;

		std::string originalContents;
		std::string appendContents;

		err::FileError err  = _sys->appendFile(appendData.getDirectory(), appendDataFileName, toAppend.getDirectory(), toAppendFileName);
		if (err::bad(err))
		{
			std::cout << "Error appending file, id: " << err::getMsg(err) << std::endl;
		}
	}

	/* Moves a file or directory to another directory
	*/
	void FileSystemHandle::move(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName)
	{
		DirectoryReference source, target;

		//Construct directories.
		if (constructDirRef(sourceDirectory, source) ||
			constructDirRef(targetDirectory, target))
			//Bad file path
			return;

		err::FileError err = _sys->move(source.getDirectory(), sourceFileName, target.getDirectory(), targetFileName);
		if (err::bad(err))
		{
			std::cout << "Error moving item, error: " << err::getMsg(err) << std::endl;
		}
	}

	void FileSystemHandle::listDirectory() {
		std::vector<std::string> list;
		err::FileError err = _sys->listDir(_dir.getDirectory(), list);

		if (err::good(err))
		{
			std::cout << "Listing directory:" << std::endl;

			size_t size = list.size();

			for (int i = 0; i < size; i++)
			{
				std::cout << list[i] << std::endl;
			}
		}
		else
		{
			std::cout << "Error listing files, error: " << err::getMsg(err) << std::endl;
		}
	}

	std::string FileSystemHandle::getWorkingPath()
	{
		return _dir.getDirectoryString();
	}

	void FileSystemHandle::format()
	{
		_dir.format();
		_sys->format();
	}

	/* Creates an image of file system
	*/
	void FileSystemHandle::createImage(const std::string fileName)
	{
		_sys->writeImage(fileName);
	}

	/* Reads an image of file system
	*/
	void FileSystemHandle::readImage(const std::string fileName)
	{
		_sys = FileSystem::readImage(fileName);
	}

	std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr) {
		os << dr._dir;
		return os;
	}
}