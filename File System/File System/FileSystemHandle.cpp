#include "FileSystemHandle.h"
#include"FileError.h"
#include<sstream>

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
	bool FileSystemHandle::constructDirRef(const std::string& str, DirectoryReference& ref)
	{
		ref.directoryFromString(_dir, str, *_sys);

		return false;
	}

	bool FileSystemHandle::constructDirRefWithFile(const std::string& str, DirectoryReference& ref, std::string& filename)
	{
		filename = ref.directoryAndFileFromString(_dir, str, *_sys);

		return false;
	}

	void FileSystemHandle::createFolder(const std::string& name) {
		err::FileError error = _sys->createFolder(_dir.getDirectory(), name);

		if (err::isError(error))
		{
			throw(error);
		}
	}
	/* Create a file
	*/
	void FileSystemHandle::createFile(const std::string& file_name, char access, const std::string& data) {
		err::FileError error = _sys->createFile(_dir.getDirectory(), file_name, access, data);
		if (err::isError(error))
		{
			throw(error);
		}
	}
	/* Remove a file or directory
	*/
	void FileSystemHandle::remove(const std::string& path)
	{
		err::FileError error = _sys->remove(_dir.getDirectory(), path);
		if (err::isError(error))
		{
			throw(error);
		}
	}
	/* Get the data in a file
	*/
	std::string FileSystemHandle::getFile(const std::string& file_name) {
		std::string data;
		std::string fileName;

		constructDirRefWithFile(file_name, _dir, fileName);

		err::FileError error = _sys->getFile(_dir.getDirectory(), fileName, data);
		if (err::bad(error))
			throw error;
		return data;
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
		err::FileError err = _sys->copy(source.getDirectory(), sourceFileName, target.getDirectory(), targetFileName);
		if (err::isError(err))
		{
			throw(err);
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

		err::FileError err  = _sys->appendFile(appendData.getDirectory(), appendDataFileName, toAppend.getDirectory(), toAppendFileName);
		if (err::isError(err))
		{
			throw(err);
		}
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
		if (err::isError(err))
		{
			throw(err);
		}
	}

	std::string FileSystemHandle::listDirectory() {
		std::stringstream stream;
		err::FileError err = _sys->listDir(_dir.getDirectory(), stream);
		//Check error
		if (isError(err))
			throw(err);
		//Check empty
		if (stream.tellp() > 0)
			return stream.str();
		return "Empty directory.\n";
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
		if (err::isError(err))
			throw err;
	}

	std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr) {
		os << dr._dir;
		return os;
	}
}