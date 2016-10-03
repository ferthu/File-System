#include "DirectoryTree.h"


namespace file {

	DirectoryTree::DirectoryTree(std::unique_ptr<Directory> root)
		: _root(std::move(root))
	{
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

}
