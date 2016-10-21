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
		std::unique_ptr<FileSystem> _sys;

		/* Append or Construct a directory reference from the current directory
		str		<<	Directory string
		ref		<<	Directory reference that is appended
		return	>>	True if it was a bad directory. False if everything is OK.
		*/
		bool constructDirRef(const std::string& str, DirectoryReference& ref);

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
		void createFile(const std::string& file_name, char access, const std::string& data);
		/* Remove a file or directory
		*/
		void remove(const std::string& path);
		/* Get the data in a file
		*/
		void printFile(const std::string& file_name);
		/* Copies a file from a directory to another
		*/
		void copyFile(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName);

		/* Appends a file to another
		*/
		void appendFile(const std::string& toAppendDirectory, const std::string& toAppendFileName, const std::string& appendDataDirectory, const std::string& appendDataFileName);

		/* Moves a file or directory to another directory
		*/
		void move(const std::string& sourceDirectory, const std::string& sourceFileName, const std::string& targetDirectory, const std::string& targetFileName);

		/* Returns current working directory
		*/
		std::string getWorkingPath();

		/* Creates an image of file system
		*/
		void createImage(const std::string fileName);

		/* Reads an image of file system
		*/
		void readImage(const std::string fileName);

		/* Empties file system
		*/
		void format();

		/* Output working directory to the stream
		*/
		friend std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr);
	};

}

