#include "FileTree.h"


namespace file {

	FileTree::FileTree(std::unique_ptr<FileNode>& root)
		: _root(std::move(root))
	{
	}


	FileTree::~FileTree()
	{
	}


	/* Get a node in the file tree.
	*/
	DirectoryAccess FileTree::accessDirectory(const std::vector<std::string>& directory) {
		/* Call the const version of the function.
		*/
		return const_cast<const FileTree*>(this)->accessDirectory(directory);
	}
	/* Get a node in the file tree.
	*/
	const DirectoryAccess FileTree::accessDirectory(const std::vector<std::string>& directory) const {
		unsigned int lvl = 0;
		return _root->accessDirectory(directory, lvl);
	}

}
