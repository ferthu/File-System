#include "filesystem.h"

FileSystem::FileSystem()
	: _root(std::unique_ptr<file::Directory>(new file::Directory("Root"))) {

}

FileSystem::~FileSystem() {

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
	if (dir.access()) {
		return dir._directory->removeDirectory(folder_name);
	}
	return dir.getError();
}
