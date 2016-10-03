#include "FileNode.h"

namespace file {

	FileNode::FileNode()
	{
	}


	FileNode::~FileNode()
	{
	}

	/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.
	*/
	DirectoryAccess FileNode::accessDirectory(const std::vector<std::string>& directory, unsigned int& traversal_lvl) {
		return DirectoryAccess(traversal_lvl, this, DirectoryAccess::ACCESS);
	}
}