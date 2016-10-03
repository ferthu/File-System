#pragma once

#include"FileError.h"

namespace file {

	/* Forward Declaration
	*/
	class Directory;

	/* Directory access result when querying for a directory. Contains directory link on successfull access.
	*/
	class DirectoryAccess {
	public:

		/* The directory level reached. Determines the directory level an error occured.
		*/
		unsigned int _lvl;
		/* Link to the directory accessed, null if error occured.
		*/
		Directory* _directory;
		/* The error that occured
		*/
		err::FileError _error;

		DirectoryAccess(unsigned int lvl, Directory* directory, err::FileError error) :
			_directory(directory), _lvl(lvl), _error(error)
		{		}
		/* Function that determines if the directory was successully accessed
		*/
		bool access() const {
			/* Verify no error
			*/
			return _error == err::FileError::SUCCESS;
		}
		/* Get the error
		*/
		err::FileError getError() {
			return _error;
		}
	};

}