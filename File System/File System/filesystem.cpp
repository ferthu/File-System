#include "filesystem.h"

#include"BinaryFileWriter.h"
#include"BinaryFileReader.h"
#include"VirtualReader.h"
#include"VirtualWriter.h"

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
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		file::type::Dir type = dir->getType(name);
		if (type == file::type::File)
			return rmFile(dir, name);
		else if (type == file::type::Folder)
			return rmFolder(dir, name);
		return err::NOT_FOUND;
	}
	return dir.getError();
}

err::FileError FileSystem::move(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& move_dir, const std::string& move_name) {

	//Access first directory
	file::DirectoryAccess f_dir = _root.accessDirectory(from_dir);
	if (!f_dir.access())
		return f_dir.getError();
	//Access second directory
	file::DirectoryAccess m_dir = _root.accessDirectory(move_dir);
	if (!m_dir.access())
		return m_dir.getError();

	file::FileReference fr;
	file::FileHeader fh;
	f_dir->getFile(from_name, fr);
	file::VirtualReader reader(_manager.getDisk());
	reader.readHeader(fr._block, fh);
	if (!fh.isReadable())
		return err::NO_READ_ACCESS;

	//Move
	return f_dir->moveChild(from_name, move_name, *m_dir._directory);
}

/* This function will get all the files and folders in the specified folder */
err::FileError FileSystem::listDir(const std::vector<std::string>& directory, std::vector<std::string>& list) const {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		std::vector<std::string> tmp;
		//Get directories
		tmp = dir->getDirectoryNames();
		list.assign(tmp.begin(), tmp.end());

		// add slash to separate files from directories
		for (int i = 0; i < list.size(); i++)
			list[i].append("/");

		//Get files
		tmp = dir->getFileNames();
		list.insert(list.end(), tmp.begin(), tmp.end());
		return err::SUCCESS;
	}
	return dir.getError();
}

#pragma endregion

#pragma region Directory Functions

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

err::FileError FileSystem::listDirOnly(const std::vector<std::string>& directory, std::vector<std::string>& list) const
{
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		std::vector<std::string> tmp;
		//Get directories
		tmp = dir->getDirectoryNames();
		list.assign(tmp.begin(), tmp.end());
		return err::SUCCESS;
	}
	return dir.getError();
}

#pragma endregion

#pragma region File Functions

err::FileError FileSystem::createFile(const std::vector<std::string>& directory, const std::string& file_name, char access, const std::string& data) {
	file::DirectoryAccess dir = _root.accessDirectory(directory);
	if (dir.access()) {
		//File reference operated on
		file::FileReference ref;
		/* Verify that file does not already exist
		*/
		if (err::good(dir->getFile(file_name, ref))) {
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
			err::FileError error = _manager.writeFile(file_name, data, access, ref);
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
	error = t_dir->getFile(from_name, ref_t);
	if (err::bad(error))
		return error;

	//Append file, if failed leave directory tree unchanged
	error = _manager.appendFile(ref_f, ref_t);
	if (err::bad(error))
		return error;

	//Update directories
	f_dir->removeFile(ref_f); //Not found error ignored.
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
	//Get file to copy
	err::FileError error = f_dir->getFile(from_name, ref_f);
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
/* Read a copy of the file system from an image on the disk.
*/
std::unique_ptr<FileSystem> FileSystem::readImage(const std::string& name) {
	mf::BinaryFileReader reader;
	if (reader.openFile(name)) {
		//Read directory tree
		file::DirectoryTree root = file::DirectoryTree::readFromStream(reader);
		//Read block manager
		file::BlockManager manager = file::BlockManager::readFromStream(reader);
		return std::unique_ptr<FileSystem>(new FileSystem(root, manager));
	}
	return std::unique_ptr<FileSystem>();
}

#pragma endregion

void FileSystem::format()
{
	_root.format();
	_manager.format();
}