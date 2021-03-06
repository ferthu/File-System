#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include"DirectoryTree.h"
#include"BlockManager.h"
#include<sstream>

class FileSystem
{
private:
	/* The directory tree.
	*/
	file::DirectoryTree _root;
	file::BlockManager _manager;
    // Here you can add your own data structures

	/* Remove a file
	*/
	err::FileError rmFile(file::DirectoryAccess& dir, const std::string& file_name);
	/* Remove a folder
	*/
	err::FileError rmFolder(file::DirectoryAccess& dir, const std::string& folder_name);
public:
    FileSystem();
	FileSystem(file::DirectoryTree& tree, file::BlockManager& manager);
    ~FileSystem();

    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */

	/* Check if the directory exist
	*/
	err::FileError directoryExists(const std::vector<std::string>& directory) const;
    /* Creates a folder in the filesystem 
	*/
	err::FileError createFolder(const std::vector<std::string>& directory, const std::string& folder_name);
	
	/* Removes a folder in the filesystem 
	*/
	err::FileError removeFolder(const std::vector<std::string>& directory, const std::string& folder_name);

    /* This function creates a file in the filesystem 
	*/
	err::FileError createFile(const std::vector<std::string>& directory, const std::string& file_name, char access, const std::string& data);
	/* This function creates a file in the filesystem
	*/
	err::FileError getFile(const std::vector<std::string>& directory, const std::string& file_name, std::string& data);
    
	/* Removes a file in the filesystem 
	*/
	err::FileError removeFile(const std::vector<std::string>& directory, const std::string& file_name);
	/* Removes a folder or file in the filesystem
	*/
	err::FileError remove(const std::vector<std::string>& directory, const std::string& name);
	/* Moves a folder or file in the system
	*/
	err::FileError move(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& move_dir, const std::string& move_name);

	/* This function will get all the files and folders in the specified directory and append it to the stream */
    err::FileError listDir(const std::vector<std::string>& directory, std::stringstream& info) const;

	/* Set the specific rights of the file
	directory	<<	Directory of the file to append.
	file_name	<<	Name of the file to be appended, removed on success.
	status		<<	Access rights. (readbit,writebit) -> 0:No access, 1: Write Only, 2: Read Only, 2<: Read & Write	
	*/
	err::FileError setRights(const std::vector<std::string>& directory, const std::string& file_name, char status);

	/* Append file data from a file to the end of another. Removing file that is appended to the other.
	from_dir	<<	Directory of the file to append.
	from_name	<<	Name of the file to be appended, removed on success.
	to_dir		<<	Directory of the file the other file is appended onto.
	to_name		<<	Name of the file to be appended with the other file.
	return		>>	Returns if any error occured
	*/
	err::FileError appendFile(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& to_dir, const std::string& to_name);
	/* Copy a file from a specified directory and file name, to a specified directory and file copy name
	from_dir	<<	Directory of the file to copy
	from_name	<<	Name of the file to copy
	copy_dir	<<	Directory to receive the copied file
	copy_name	<<	Name of the copied file
	return		>>	Returns if any error occured
	*/
	err::FileError copy(const std::vector<std::string>& from_dir, const std::string& from_name, const std::vector<std::string>& copy_dir, const std::string& copy_name);

	/* Write the file system as an image from the disk
	*/
	void writeImage(const std::string& name);
	/* Read a copy of the file system from an image on the disk.
	*/
	static std::unique_ptr<FileSystem> readImage(const std::string& name);

	/* Removes all files and directories
	*/
	void format();

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H
