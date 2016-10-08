#pragma once

#include<vector>
#include<string>
#include"File.h"
#include"DirectoryAccess.h"
#include<memory>
#include"FileReference.h"
#include"IDirectoryReference.h"
#include"BinaryFileReader.h"
#include"BinaryFileWriter.h"

namespace file {

	/* The file node
	*/
	class Directory : public IDirectoryReference
	{
	private:
		/* Name of this directory level
		*/
		std::string _name;
		/* References to the child directories
		*/
		std::vector<std::unique_ptr<Directory>> _directories;
		/* References to file headers stored in memory.
		*/
		std::vector<FileReference> _files;
		/* Compares a directory name
		*/
		bool compareDirectoryName(const std::string& dir_name);
	public:
		Directory(const std::string& name);
		virtual ~Directory();

		/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.  
		*/
		DirectoryAccess accessDirectory(const std::vector<std::string>& directory, unsigned int traversal_lvl);
		/* Add a child directory to the node
		*/
		virtual void addDirectory(std::unique_ptr<Directory>& child);
		/* Remove a child directory in the node
		name	<<	Name of the directory to remove
		return	>>	If the directory/folder was successfully removed
		*/
		virtual err::FileError removeDirectory(const std::string& name);
		/* Get a file
		name		<<	Name of the file
		reference	>>	Return reference of the found file
		return		>>	Returns if file was successfully found or if an error occured.
		*/
		virtual err::FileError getFile(const std::string& name, FileReference& reference);
		/* Add a file reference. If a reference with same name exists old reference is deleted.
		ref		<<	Reference to the file
		*/
		virtual void addFile(const FileReference& ref);
		/* Remove a file
		ref		<<	Reference to the file
		return	>>	Success if no error occured.
		*/
		virtual err::FileError removeFile(const FileReference& ref);
		/* Get the file names
		*/
		virtual std::vector<std::string> getFileNames();
		/* Get the names of the child directories
		*/
		virtual std::vector<std::string> getDirectoryNames();


		/* Write directory to stream
		*/
		void writeToStream(mf::BinaryFileWriter& writer);
		/* Read a Directory element from the stream
		*/
		static std::unique_ptr<Directory> readFromStream(mf::BinaryFileReader& reader);
	};

}

