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
	DirectoryAccess Directory::accessDirectory(const std::vector<std::string>& directory, unsigned int& traversal_lvl) {
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
				//Move the last directory to the slot and pop last element.
				_directories[i] = std::move(_directories.back());
				_directories.pop_back();
				return err::SUCCESS;
			}
		}
		return err::FOLDER_DOES_NOT_EXIST;
	}
}