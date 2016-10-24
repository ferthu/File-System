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

		const char delimChar = '/';

		/* Append or Construct a directory reference from the current directory
		str		<<	Directory string
		ref		<<	Directory reference that is appended
		return	>>	True if it was a bad directory. False if everything is OK.
		*/
		bool constructDirRef(const std::string& str, DirectoryReference& ref);

		/* Append or Construct a directory and file reference from the current directory
		str		 <<	Directory string
		ref		 <<	Directory reference that is appended
		filename >>	Returns name of file specified in string
		return	 >>	True if it was a bad directory. False if everything is OK.
		*/
		bool constructDirRefWithFile(const std::string& str, DirectoryReference& ref, std::string& filename);

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
		std::string listDirectory(const std::string& dir);
		/* Create a file
		*/
		void createFile(const std::string& file_name, char access, const std::string& data);
		/* Remove a file or directory
		*/
		void remove(const std::string& path);
		/* Get the data in a file
		*/
		std::string getFile(const std::string& file_name);
		/* Copies a file from a directory to another
		*/
		void copyFile(const std::string& sourceDirectory, const std::string& targetDirectory);

		/* Appends a file to another
		*/
		void appendFile(const std::string& toAppendDirectory, const std::string& appendDataDirectory);

		/* Moves a file or directory to another directory
		*/
		void move(const std::string& sourceDirectory, const std::string& targetDirectory);

		/* Returns current working directory
		*/
		std::string getWorkingPath();

		/* Creates an image of file system
		*/
		void createImage(const std::string fileName);

		/* Reads an image of file system
		return	>> If succesfull load
		*/
		bool readImage(const std::string fileName);

		/* Empties file system
		*/
		void format();

		/* Sets access rights of a file
		*/
		void setRights(const std::string& fileDir, std::string status);

		/* Output working directory to the stream
		*/
		friend std::ostream& operator<<(std::ostream& os, const FileSystemHandle& dr);
	};

}

