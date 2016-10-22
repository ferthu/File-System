#pragma once

#include<memory>
#include<string>
#include<vector>
#include"FileReference.h"
#include"FileError.h"
#include"DirectoryType.h"
#include<sstream>

namespace file {

	/* Forward declaration
	*/
	class Directory;
	
	/* Interface for referencing a directory object.
	*/
	class IDirectoryReference {
	public:
		/* Add a child directory to the node
		*/
		virtual void addDirectory(std::unique_ptr<Directory>& child) = 0;
		/* Remove a child directory in the node
		name	<<	Name of the directory to remove
		return	>>	If the directory/folder was successfully removed
		*/
		virtual err::FileError removeDirectory(const std::string& name) = 0;
		/* Get a file
		name		<<	Name of the file
		reference	>>	Return reference of the found file
		return		>>	Returns if file was successfully found or if an error occured.
		*/
		virtual err::FileError getFile(const std::string& name, FileReference& reference) = 0;
		/* Add a file reference. If a reference with same name exists old reference is deleted.
		ref		<<	Reference to the file
		*/
		virtual void addFile(const FileReference& ref) = 0;
		/* Remove a file
		ref		<<	Reference to the file
		return	>>	Success if no error occured.
		*/
		virtual err::FileError removeFile(const FileReference& ref) = 0;
		/* Get the file names
		*/
		virtual void getFileInfo(std::stringstream& stream) = 0;
		/* Get the info of the child directories
		*/
		virtual void getDirectoryInfo(std::stringstream& stream) = 0;

		/* Get if the name is a directory or a file type
		*/
		virtual type::Dir getType(const std::string& name) = 0;
		/* Move ownership of a child directory or file
		*/
		virtual err::FileError moveChild(const std::string& name, const std::string& new_name, IDirectoryReference& ref) = 0;
		/* Virtual destructor
		*/
		virtual ~IDirectoryReference() {};
	};

}