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
		/* Size in bytes of all files in this and following directories.
		*/
		unsigned int _byte_size;
		/* Pointer reference to the parent can be null (in root node)
		*/
		Directory* _parent;
		/* Update a change in byte size
		difference	<<	Byte size the directory is changed with
		*/
		void updateSize(int difference);

		/* References to the child directories
		*/
		std::vector<std::unique_ptr<Directory>> _directories;
		/* References to file headers stored in memory.
		*/
		std::vector<FileReference> _files;
		/* Compares a directory name
		*/
		bool compareDirectoryName(const std::string& dir_name);
		/* Find a directory. Returning the index in the directory list.
		*/
		int getDirectory(const std::string& dir_name);
		/* Removes a specific directory from index. 
		i			<<	Index to remove
		byte_size	<<	The byte size of the directory 
		*/
		void removeDirectory(int i, int byte_size);
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
		virtual void getFileInfo(std::stringstream& stream);
		/* Get the info of the child directories
		*/
		virtual void getDirectoryInfo(std::stringstream& stream);

		/* Get if the name is a directory or a file type
		*/
		virtual type::Dir getType(const std::string& name);
		/* Move ownership of a child directory or file
		*/
		virtual err::FileError moveChild(const std::string& name, const std::string& new_name, IDirectoryReference& move_to);

		/* Write directory to stream
		*/
		void writeToStream(mf::BinaryFileWriter& writer);
		/* Read a Directory element from the stream
		*/
		static std::unique_ptr<Directory> readFromStream(mf::BinaryFileReader& reader);
	};

}

