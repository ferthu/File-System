#pragma once

#include"filesystem.h"
#include"DirectoryReference.h"

namespace file {

	/*	Object used as interface between the program and the file system. Containing the current working directory.
	*/
	class FileSystemHandle
	{
	private:
		DirectoryReference _dir;
		FileSystem _sys;
	public:
		FileSystemHandle();
		~FileSystemHandle();
		/* Create a folder
		*/
		void createFolder(const std::string& name);
		/* Move to working directory
		*/
		void cd(const std::string& str);
		/* List directory items
		*/
		void listDirectory();
		/* Create a file
		*/
		void createFile(const std::string& file_name, const std::string& data);
		/* Remove a file
		*/
		void removeFile(const std::string& file_name);
		/* Get the data in a file
		*/
		void printFile(const std::string& file_name);
		/* Copies a file from a directory to another
		*/
		void copyFile(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName);

		/* Appends a file to another
		*/
		void appendFile(const std::string& toAppendDirectory, const std::string& toAppendFileName, const std::string& appendDataDirectory, const std::string& appendDataFileName);

		/* Moves a file to another directory
		*/
		void moveFile(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName);

		/* Empties file system
		*/
		void format();

		/* Output working directory to the stream
		*/
		friend std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr);
	};

}

