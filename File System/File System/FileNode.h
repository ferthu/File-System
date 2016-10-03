#pragma once

#include<vector>
#include<string>
#include"File.h"

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
	};

}

