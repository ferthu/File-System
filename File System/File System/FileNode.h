#pragma once

#include<vector>
#include<string>
#include"File.h"
#include"DirectoryAccess.h"

namespace file {

	/* The file node
	*/
	class FileNode
	{
	private:
		/* References to the child directories
		*/
		std::vector<FileNode*> _directories;
		/* Files contained in the directory
		*/
		std::vector<File> _files;
	public:
		FileNode();
		virtual ~FileNode();

		/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.  
		*/
		DirectoryAccess accessDirectory(const std::vector<std::string>& directory, unsigned int& traversal_lvl);
		
	};

}

