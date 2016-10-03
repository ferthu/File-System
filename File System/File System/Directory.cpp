#include "Directory.h"

namespace file {

	Directory::Directory()
	{
	}


	Directory::~Directory()
	{
	}

	/* Compares a directory name
	*/
	bool Directory::compareDirectoryName(const std::string& dir_name) {
		return true;
	}
	/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.
	*/
	DirectoryAccess Directory::accessDirectory(const std::vector<std::string>& directory, unsigned int& traversal_lvl) {
		for (size_t i = 0; i < _directories.size(); i++) {

		}
		return DirectoryAccess(traversal_lvl, this, DirectoryAccess::ACCESS);
	}
}