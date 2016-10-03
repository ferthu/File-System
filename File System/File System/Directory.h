#pragma once

#include<vector>
#include<string>
#include"File.h"
#include"DirectoryAccess.h"

namespace file {

	/* The file node
	*/
	class Directory
	{
	private:
		/* Name of this 
		*/
		std::string _directory_name;
		/* References to the child directories
		*/
		std::vector<Directory*> _directories;
		/* Files contained in the directory
		*/
		std::vector<File> _files;
		/* Compares a directory name
		*/
		bool compareDirectoryName(const std::string& dir_name);
	public:
		Directory();
		virtual ~Directory();

		/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.  
		*/
		DirectoryAccess accessDirectory(const std::vector<std::string>& directory, unsigned int& traversal_lvl);
		
	};

}

