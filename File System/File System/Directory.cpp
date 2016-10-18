#include "Directory.h"

namespace file {

	Directory::Directory(const std::string& name)
		: _name(name), _directories(), _files()
	{
	}


	Directory::~Directory()
	{
	}

	/* Compares a directory name
	*/
	bool Directory::compareDirectoryName(const std::string& dir_name) {
		return dir_name == _name;
	}
	/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.
	*/
	DirectoryAccess Directory::accessDirectory(const std::vector<std::string>& directory, unsigned int traversal_lvl) {
		/* Check if destination directory is reached
		*/
		if(traversal_lvl == directory.size())
			return DirectoryAccess(traversal_lvl, this, err::SUCCESS);
		/* Traverse the tree
		*/
		for (size_t i = 0; i < _directories.size(); i++) {
			if (_directories[i]->compareDirectoryName(directory[traversal_lvl]))
				return _directories[i]->accessDirectory(directory, ++traversal_lvl);
		}
		/*	No child directory with specified name exists
		*/
		return DirectoryAccess(traversal_lvl + 1, nullptr, err::DIRECTORY_DOES_NOT_EXIST);
	}
	/* Add a child directory to the node
	*/
	void Directory::addDirectory(std::unique_ptr<Directory>& child) {
		_directories.push_back(std::move(child));
	}
	/* Remove a child directory in the node
	name	<<	Name of the directory to remove
	return	>>	If the directory exists
	*/
	err::FileError Directory::removeDirectory(const std::string& name) {
		for (size_t i = 0; i < _directories.size(); i++) {
			if (_directories[i]->compareDirectoryName(name)) {
				_directories.erase(_directories.begin() + i);
				return err::SUCCESS;
			}
		}
		return err::FOLDER_DOES_NOT_EXIST;
	}
	/* Get a file
	name		<<	Name of the file
	reference	>>	Return reference of the found file
	return		>>	Returns if file was successfully found or if an error occured.
	*/
	err::FileError Directory::getFile(const std::string& name, FileReference& reference) {
		for (size_t i = 0; i < _files.size(); i++) {
			if (_files[i] == name) {
				reference = _files[i];
				return err::SUCCESS;
			}
		}
		return err::FILE_NOT_FOUND;
	}
	/* Add a file reference. If a reference with same name exists old reference is deleted.
	ref		<<	Reference to the file
	*/
	void Directory::addFile(const FileReference& ref) {
		FileReference tmp;
		//Remove any existing file
		removeFile(ref);
		//Add new reference
		_files.push_back(ref);
	}
	/* Remove a file
	ref		<<	Reference to the file
	return	>>	Success if no error occured.
	*/
	err::FileError Directory::removeFile(const FileReference& ref) {
		for (size_t i = 0; i < _files.size(); i++) {
			if (_files[i] == ref._name) {
				_files.erase(_files.begin() + i);
				return err::SUCCESS;
			}
		}
		return err::FILE_NOT_FOUND;
	}
	/* Get the file names
	*/
	std::vector<std::string> Directory::getFileNames() {
		std::vector<std::string> names(_files.size());
		for (size_t i = 0; i < _files.size(); i++)
			names[i] = _files[i]._name;
		return names;
	}
	/* Get the names of the child directories
	*/
	std::vector<std::string> Directory::getDirectoryNames() {
		std::vector<std::string> names(_directories.size());
		for (size_t i = 0; i < _directories.size(); i++)
		{
			names[i] = _directories[i]->_name;
		}
		return names;
	}


	/* Writer to stream
	*/
	void Directory::writeToStream(mf::BinaryFileWriter& writer) {
		writer.writeString(_name);

		//Write file references
		writer.writeUInt((unsigned int)_files.size());
		for (int i = 0; i < _files.size(); i++) {
			writer.writeString(_files[i]._name);
			writer.writeInt(_files[i]._block);
		}
		//Write directories
		writer.writeUInt((unsigned int)_directories.size());
		for (int i = 0; i < _files.size(); i++)
			_directories[i]->writeToStream(writer);
	}
	std::unique_ptr<Directory> Directory::readFromStream(mf::BinaryFileReader& reader) {

		std::string name = reader.readString();
		std::unique_ptr<Directory> dir(new Directory(name));
		//Read files
		unsigned int fileNum = reader.readUInt();
		for (unsigned int i = 0; i < fileNum; i++)
			dir->addFile(FileReference(reader.readString(), reader.readInt()));
		//Read child directories
		unsigned int dirNum = reader.readUInt();
		for (unsigned int i = 0; i < dirNum; i++)
			dir->addDirectory(readFromStream(reader));
		return std::move(dir);
	}
}