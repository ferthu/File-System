#include "filesystem.h"

#include"BinaryFileWriter.h"
#include"BinaryFileReader.h"
#include"VirtualReader.h"
#include"VirtualWriter.h"
#include"DirectoryReference.h"

FileSystem::FileSystem()
	: _root(std::unique_ptr<file::Directory>(new file::Directory("Root"))), _manager() {

}

FileSystem::FileSystem(file::DirectoryTree& tree, file::BlockManager& manager)
 : _root(std::move(tree)), _manager(std::move(manager)) {

}
FileSystem::~FileSystem() {

}

#pragma region Directory & File Functions
/* Removes a folder or file in the filesystem
*/
err::FileError FileSystem::remove(const std::vector<std::string>& directory, const std::string& name) {
	DirectoryReference specifiedDirectory;
	specifiedDirectory.setDirectory(directory);
	std::string foundName;
	specifiedDirectory.directoryAndFileFromString(specifiedDirectory, name, *this, foundName);
	
	file::DirectoryAccess dir = _root.accessDirectory(specifiedDirectory.getDirectory());
	if (dir.access()) {
		file::type::Dir type = dir->getType(foundName);
		if (type == file::type::File)
			return rmFile(dir, foundName);
		else if (type == file::type::Folder)
			return rmFolder(dir, foundName);
		return err::NOT_FOUND;
	}
	return dir.getError();
}

err::FileError FileSystem::move(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& move_dir, const std::string& move_name) {

	file::FileReference fr, rm;
	//Access first directory
	file::DirectoryAccess f_dir = _root.accessDirectory(from_dir);
	if (!f_dir.access())
		return f_dir.getError();

	//Verify from file is movable.
	err::FileError error = f_dir->getFile(from_name, fr);
	if (err::bad(error))
		return error;
	error = _manager.verifyAccess(fr, 3);
	if (err::bad(error))
		return error;

	//Access second directory
	file::DirectoryAccess m_dir = _root.accessDirectory(move_dir);
	if (!m_dir.access())
		return m_dir.getError();

	//Remove any existing file with same name.
	error = m_dir->getFile(move_name, rm);
	if (err::good(error)) {
		//Verify file isn't moved onto itself before removing target.
		if (f_dir._directory == m_dir._directory && move_name == from_name) 
			return err::SUCCESS;
		//Remove file:
		else {
			error = rmFile(m_dir, move_name);
			if (err::bad(error))
				return error;
		}
	}

	//Move
	return f_dir->moveChild(from_name, move_name, *m_dir._directory);
}

/* This function will get all the files and folders in the specified directory and append it to the stream */
err::FileError FileSystem::listDir(const std::vector<std::string>& directory, std::stringstream& info) const {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		//Get directories
		dir->getDirectoryInfo(info);
		//Get files
		dir->getFileInfo(info);
		return err::SUCCESS;
	}
	return dir.getError();
}

#pragma endregion

#pragma region Directory Functions

/* Check if the directory exist
*/
err::FileError FileSystem::directoryExists(const std::vector<std::string>& directory) const
{
	file::DirectoryAccess dir = _root.accessDirectory(directory);

	return dir.getError();
}

/* Creates a folder in the filesystem */
err::FileError FileSystem::createFolder(const std::vector<std::string>& directory, const std::string& folder_name) {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		dir._directory->addDirectory(std::unique_ptr<file::Directory>(new file::Directory(folder_name)));
	}
	return dir.getError();
}
/* Removes a folder in the filesystem */
err::FileError FileSystem::removeFolder(const std::vector<std::string>& directory, const std::string& folder_name) {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	return rmFolder(dir, folder_name);
}
/* Remove a folder
*/
err::FileError FileSystem::rmFolder(file::DirectoryAccess& dir, const std::string& folder_name) {
	if (dir.access()) {
		return dir._directory->removeDirectory(folder_name);
	}
	return dir.getError();
}


#pragma endregion

#pragma region File Functions

err::FileError FileSystem::createFile(const std::vector<std::string>& directory, const std::string& file_name, char access, const std::string& data) {
	DirectoryReference specifiedDirectory;
	specifiedDirectory.setDirectory(directory);
	std::string fileName;
	specifiedDirectory.directoryAndFileFromString(specifiedDirectory, file_name, *this, fileName);

	file::DirectoryAccess dir = _root.accessDirectory(specifiedDirectory.getDirectory());
	if (dir.access()) {
		//File reference operated on
		file::FileReference ref;
		/* Verify that file does not already exist
		*/
		if (err::good(dir->getFile(fileName, ref))) {
			/* Overwrite old file
			*/
			err::FileError error = _manager.overwriteFile(ref, data, access);
			if (err::bad(error))
				return error;
			//Update reference
			dir->addFile(ref);
		}
		else {
			/* Write a new file
			*/
			err::FileError error = _manager.writeFile(fileName, data, access, ref);
			if (err::bad(error))
				return error;
			//Add the file
			dir->addFile(ref);
		}
	}
	return dir.getError();
}
/* This function creates a file in the filesystem
*/
err::FileError FileSystem::getFile(const std::vector<std::string>& directory, const std::string& file_name, std::string& data) {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		//File reference operated on
		file::FileReference ref;
		/* Verify that file does not already exist
		*/
		err::FileError error = dir->getFile(file_name, ref);
		if (err::good(error))
			return _manager.readFile(ref, data);
		return error;
	}
	return dir.getError();
}
/* Removes a file in the filesystem */
err::FileError FileSystem::removeFile(const std::vector<std::string>& directory, const std::string& file_name) {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	return rmFile(dir, file_name);
}
/* Set the specific rights of the file
directory	<<	Directory of the file to append.
file_name	<<	Name of the file to be appended, removed on success.
status		<<	Access rights. (readbit,writebit) -> 0:No access, 1: Write Only, 2: Read Only, 2<: Read & Write
*/
err::FileError FileSystem::setRights(const std::vector<std::string>& directory, const std::string& file_name, char status) {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		//File reference operated on
		file::FileReference ref;
		/* Verify that file does not already exist
		*/
		err::FileError error = dir->getFile(file_name, ref);
		if (err::good(error)) {
			_manager.writeAccess(ref, status);
			return err::SUCCESS;
		}
		return error;
	}
	return dir.getError();
}

/* Remove a file
*/
err::FileError FileSystem::rmFile(file::DirectoryAccess& dir, const std::string& file_name) {
	if (dir.access()) {
		//File reference operated on
		file::FileReference ref;
		/* Verify that file exist
		*/
		err::FileError error = dir->getFile(file_name, ref);
		if (err::bad(error))
			return error;

		//Remove file
		error = _manager.removeFile(ref);
		//File corrupted? remove directory representation
		if (error == err::CORRUPTED_FILE) {
			dir->removeFile(ref);
			return error;
		}
		//If other error occured (such as No_READ_ACCESS) reference should not be removed.
		else if (err::bad(error))
			return error;

		//Successfully removed file, delete directory representation
		return dir->removeFile(ref);
	}
	return dir.getError();
}

err::FileError FileSystem::appendFile(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& to_dir, const std::string& to_name) {
	//Access first directory
	file::DirectoryAccess f_dir = _root.accessDirectory(from_dir);
	if(!f_dir.access())
		return f_dir.getError();
	//Access second directory
	file::DirectoryAccess t_dir = _root.accessDirectory(to_dir);
	if (!t_dir.access())
		return t_dir.getError();

	file::FileReference ref_f, ref_t;
	//Get first file
	err::FileError error = f_dir->getFile(from_name, ref_f);
	if (err::bad(error))
		return error;
	//Get second file
	error = t_dir->getFile(to_name, ref_t);
	if (err::bad(error))
		return error;

	//Append file, if failed leave directory tree unchanged
	error = _manager.appendFile(ref_f, ref_t);
	if (err::bad(error))
		return error;

	//Update directories
	f_dir->removeFile(ref_f); //Not found error ignored!
	t_dir->addFile(ref_t);
	return err::SUCCESS;
}
err::FileError FileSystem::copy(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& copy_dir, const std::string& copy_name) {
	//Access first directory
	file::DirectoryAccess f_dir = _root.accessDirectory(from_dir);
	if (!f_dir.access())
		return f_dir.getError();
	//Access second directory
	file::DirectoryAccess c_dir = _root.accessDirectory(copy_dir);
	if (!c_dir.access())
		return c_dir.getError();

	file::FileReference ref_f, ref_c;
	//Check if a file is overwritten. Ignore non-exist error.
	err::FileError error = c_dir->getFile(copy_name, ref_c);

	//Get file to copy
	error = f_dir->getFile(from_name, ref_f);
	if (err::bad(error))
		return error;

	//Copy file. If error occurs nothing happends.
	error = _manager.copyFile(copy_name, ref_f, ref_c);
	if (err::bad(error))
		return error;

	//Add copy to directory tree
	c_dir->addFile(ref_c);
	return err::SUCCESS;
}

#pragma endregion

#pragma region File Image

/* Write the file system as an image from the disk
*/
void FileSystem::writeImage(const std::string& name) {
	mf::BinaryFileWriter writer;

	if (writer.openFile(name)) {
		//Write directory tree
		_root.writeToStream(writer);
		//Write block manager
		_manager.writeToStream(writer);
	}
	return;
}
/* Read a copy of the file system from an image on the disk. Null if file failed to load.
name	<<	File name
return	<<	FileSystem read from the file or null if error occured.
*/
std::unique_ptr<FileSystem> FileSystem::readImage(const std::string& name) {
	mf::BinaryFileReader reader;
	if (reader.openFile(name)) {
		try {
			//Read directory tree
			file::DirectoryTree root = file::DirectoryTree::readFromStream(reader);
			//Read block manager
			file::BlockManager manager = file::BlockManager::readFromStream(reader);
			return std::unique_ptr<FileSystem>(new FileSystem(root, manager));
		}
		catch (const std::exception& e){	
		}
	}
	return std::unique_ptr<FileSystem>();
}

#pragma endregion

void FileSystem::format()
{
	_root.format();
	_manager.format();
}