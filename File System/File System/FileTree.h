#pragma once

#include"FileNode.h"
#include"File.h"
#include<memory>

namespace file {
	
	
	/*	Wrapper containing the root node and access functions to the file tree.
	*/
	class FileTree
	{
	private:

		/* Root node.
		*/
		std::unique_ptr<FileNode> _root;

	public:
		/* Create a file tree from the root reference.
		*/
		FileTree(std::unique_ptr<FileNode>& root);
		~FileTree();
	};

}

