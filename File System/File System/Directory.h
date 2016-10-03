#pragma once

#include<vector>
#include<string>
#include"File.h"
#include"DirectoryAccess.h"
#include<memory>

namespace file {

	/* The file node
	*/
	class Directory
	{
	private:
		/* Name of this directory level
		*/
		std::string _name;
		/* References to the child directories
		*/
		std::vector<std::unique_ptr<Directory>> _directories;
		/* Files contained in the directory
		*/
		std::vector<File> _files;
		/* Compares a directory name
		*/
		bool compareDirectoryName(const std::string& dir_name);
	public:
		Directory(const std::string& name);
		virtual ~Directory();

		/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.  
		*/
		DirectoryAccess accessDirectory(const std::vector<std::string>& directory, unsigned int& traversal_lvl);
		/* Add a child directory to the node
		*/
		void addDirectory(std::unique_ptr<Directory>& child);
		/* Remove a child directory in the node
		name	<<	Name of the directory to remove
		return	>>	If the directory/folder was successfully removed
		*/
		err::FileError removeDirectory(const std::string& name);
	};

}

