#include "DirectoryTree.h"


namespace file {

	DirectoryTree::DirectoryTree(std::unique_ptr<Directory>& root)
		: _root(std::move(root))
	{
	}
	DirectoryTree::DirectoryTree(DirectoryTree&& root)
		: _root(std::move(root._root)) {

	}

	DirectoryTree& DirectoryTree::operator=(DirectoryTree&& move) {
		if (this == &move)
			return *this;
		_root = std::move(move._root);
		return *this;
	}

	DirectoryTree::~DirectoryTree()
	{
	}


	/* Get a node in the file tree.
	*/
	DirectoryAccess DirectoryTree::accessDirectory(const std::vector<std::string>& directory) {
		/* Call the const version of the function.
		*/
		return const_cast<const DirectoryTree*>(this)->accessDirectory(directory);
	}
	/* Get a node in the file tree.
	*/
	const DirectoryAccess DirectoryTree::accessDirectory(const std::vector<std::string>& directory) const {
		unsigned int lvl = 0;
		return _root->accessDirectory(directory, lvl);
	}

	/* Write to stream
	*/
	void DirectoryTree::writeToStream(mf::BinaryFileWriter& writer) {
		_root->writeToStream(writer);
	}
	/* Read from stream
	*/
	DirectoryTree DirectoryTree::readFromStream(mf::BinaryFileReader& reader) {
		return DirectoryTree(Directory::readFromStream(reader));
	}

	void DirectoryTree::format()
	{
		_root = std::unique_ptr<file::Directory>(new file::Directory("Root"));
	}
}
