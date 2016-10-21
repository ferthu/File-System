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
			ref.directoryFromString(_dir, str, *_sys);
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

	bool FileSystemHandle::constructDirRefWithFile(const std::string& str, DirectoryReference& ref, std::string& filename)
	{
		try
		{
			filename = ref.directoryAndFileFromString(_dir, str, *_sys);
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
	void FileSystemHandle::createFile(const std::string& file_name, const std::string& data) {
		err::FileError error = _sys->createFile(_dir.getDirectory(), file_name, data);
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
		std::string fileName;

		constructDirRefWithFile(file_name, _dir, fileName);

		err::FileError error = _sys->getFile(_dir.getDirectory(), fileName, data);
		if (err::good(error))
			std::cout << data << std::endl;
		else 
			std::cout << "Couldn't access file, error: " << err::getMsg(error) << std::endl;
	}
	
	void FileSystemHandle::copyFile(const std::string& sourceDirectory, const std::string& targetDirectory)
	{
		DirectoryReference source, target;

		std::string sourceFileName, targetFileName;

		//Construct directories.
		if (constructDirRefWithFile(sourceDirectory, source, sourceFileName) ||
			constructDirRefWithFile(targetDirectory, target, targetFileName))
			//Bad file path
			return;

		std::string fileContents;
		err::FileError err = _sys->getFile(source.getDirectory(), sourceFileName, fileContents);
		if (err::good(err))
		{
			err = _sys->createFile(target.getDirectory(), targetFileName, fileContents);
			if (err::bad(err))
			{
				std::cout << "Error copying file: " << err::getMsg(err) << std::endl;
			}
		}
		else
		{
			std::cout << "Error reading file: " << err::getMsg(err) << std::endl;
		}
	}

	void FileSystemHandle::appendFile(const std::string& toAppendDirectory, const std::string& appendDataDirectory)
	{
		DirectoryReference toAppend, appendData;

		std::string toAppendFileName, appendDataFileName;

		//Construct directories.
		if (constructDirRefWithFile(toAppendDirectory, toAppend, toAppendFileName) ||
			constructDirRefWithFile(appendDataDirectory, appendData, appendDataFileName))
			//Bad file path
			return;

		std::string originalContents;
		std::string appendContents;

		// read original file
		err::FileError err = _sys->getFile(toAppend.getDirectory(), toAppendFileName, originalContents);
		if (err::good(err))
		{
			// read append data
			err::FileError err = _sys->getFile(appendData.getDirectory(), appendDataFileName, appendContents);
			if (err::good(err))
			{
				// remove original file
				err = _sys->removeFile(toAppend.getDirectory(), toAppendFileName);
				if (err::good(err))
				{
					// create new appended file
					std::string newContents = originalContents + appendContents;
					err = _sys->createFile(toAppend.getDirectory(), toAppendFileName, newContents);
					if (err::bad(err))
						std::cout << "Error creating appended file, error: " << err << std::endl;
				}
				else
					std::cout << "Error replacing file: " << err::getMsg(err) << std::endl;
			}
			else
				std::cout << "Error reading second file: " << err::getMsg(err) << std::endl;
		}
		else
			std::cout << "Error reading first file: " << err::getMsg(err) << std::endl;

	}

	/* Moves a file or directory to another directory
	*/
	void FileSystemHandle::move(const std::string& sourceDirectory, const std::string& targetDirectory)
	{
		DirectoryReference source, target;

		std::string sourceFileName, targetFileName;

		//Construct directories.
		if (constructDirRefWithFile(sourceDirectory, source, sourceFileName) ||
			constructDirRefWithFile(targetDirectory, target, targetFileName))
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


	void FileSystemHandle::setRights(const std::string& fileDir, std::string status)
	{
		std::string filename;
		constructDirRefWithFile(fileDir, _dir, filename);

		int rights = std::stoi(status);

		err::FileError err = _sys->setRights(_dir.getDirectory(), filename, rights);
		if (err::bad(err))
			throw err;
	}

	std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr) {
		os << dr._dir;
		return os;
	}
}