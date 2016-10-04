#pragma once

namespace file {

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
		/* Remove a file
		ref		<<	Reference to the file
		return	>>	Success if no error occured.
		*/
		virtual err::FileError removeFile(const FileReference& ref) = 0;
		/* Get the file names
		*/
		virtual std::vector<std::string> getFileNames() = 0;
		/* Get the names of the child directories
		*/
		virtual std::vector<std::string> getDirectoryNames() = 0;
		/* Virtual destructor
		*/
		virtual ~IDirectoryReference() {};
	};

}