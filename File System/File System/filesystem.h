#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include"DirectoryTree.h"

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;
	/* The directory tree.
	*/
	file::DirectoryTree _root;
    // Here you can add your own data structures
public:
    FileSystem();
    ~FileSystem();

    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */


    /* Creates a folder in the filesystem */
	err::FileError createFolder(const std::vector<std::string>& directory, const std::string& folder_name);
    /* Removes a folder in the filesystem */
	err::FileError removeFolder(const std::vector<std::string>& directory, const std::string& folder_name);

    /* This function creates a file in the filesystem */
	err::FileError createFile(const std::vector<std::string>& directory, const std::string& file_name, const std::vector<char>& data);
    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H
