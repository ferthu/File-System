#pragma once

#include"Directory.h"
#include"File.h"
#include<memory>

namespace file {
	
	
	/*	Wrapper containing the root node and access functions to the file tree.
	*/
	class DirectoryTree
	{
	private:

		/* Root node.
		*/
		std::unique_ptr<Directory> _root;

	public:
		/* Create a file tree from the root reference.
		*/
		DirectoryTree(std::unique_ptr<Directory> root);
		DirectoryTree(const DirectoryTree& root) = delete;
		~DirectoryTree();

		/* Get a node in the file tree.
		*/
		DirectoryAccess accessDirectory(const std::vector<std::string>& directory);
		/* Get a node in the file tree.
		*/
		const DirectoryAccess accessDirectory(const std::vector<std::string>& directory) const;

		

	};

}

