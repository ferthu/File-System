#pragma once


namespace err {
	/* Enum of the errors that can occur in the system.
	*/
	enum FileError {
		SUCCESS = 0,
		/* A specified directory did not exist at the directory level.
		*/
		DIRECTORY_DOES_NOT_EXIST = 1,
		FOLDER_DOES_NOT_EXIST = 2
	};
}