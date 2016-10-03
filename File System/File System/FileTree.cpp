#include "FileTree.h"


namespace file {

	FileTree::FileTree(std::unique_ptr<FileNode>& root)
		: _root(std::move(root))
	{
	}


	FileTree::~FileTree()
	{
	}

}
