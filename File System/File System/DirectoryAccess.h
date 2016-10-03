#pragma once

namespace file {

	/* Forward Declaration
	*/
	class FileNode;

	/* Directory access result when querying for a directory. Contains directory link on successfull access.
	*/
	class DirectoryAccess {
	public:
		/* Errors that can occur when accessing a directory.
		*/
		enum Error
		{
			ACCESS = 0,
			/* A specified directory did not exist at the directory level.
			*/
			DIRECTORYDOESNOTEXIST = 1
		};

		/* The directory level reached. Determines the directory level an error occured.
		*/
		unsigned int _lvl;
		/* Link to the directory accessed, null if error occured.
		*/
		FileNode* _directory;
		/* The error that occured
		*/
		Error _error;

		DirectoryAccess(unsigned int lvl, FileNode* directory, Error error) :
			_directory(directory), _lvl(lvl), _error(error)
		{		}
		/* Function that determines if the directory was successully accessed
		*/
		bool access() const {
			/* Verify no error
			*/
			return _error == Error::ACCESS;
		}
	};

}