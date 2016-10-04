#pragma once


namespace err {
	/* Enum of the errors that can occur in the system.
	*/
	enum FileError {
		SUCCESS = 0,
		/* A specified directory did not exist at the directory level.
		*/
		DIRECTORY_DOES_NOT_EXIST = 1,
		/* Called if a specific folder is called for removal but it did not exist.
		*/
		FOLDER_DOES_NOT_EXIST = 2,
		/* Called if a specific file is not found in a directory
		*/
		FILE_NOT_FOUND = 3
	};
}