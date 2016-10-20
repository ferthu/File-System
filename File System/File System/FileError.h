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
		FILE_NOT_FOUND = 3,
		/* New file tried to be created with the same name as an existing file.
		*/
		FILE_ALREADY_EXIST = 4,
		/* Occurs if error occured in file system. Where a non-existing file reference was sent to the manager.
		*/
		CORRUPTED_FILE = 5,
		/* Not enough available memory
		*/
		MEMORY_OVERFLOW = 6,
		/* A bad block was sent for release to memory manager 
		*/
		CORRUPTED_BLOCK = 7,

		/* Stream is not opened or it is closed.
		*/
		STREAM_CLOSED = 19,
		/* Not allowed to read the file.
		*/
		NO_READ_ACCESS = 21,
		/* Not allowed to write to the file.
		*/
		NO_WRITE_ACCESS = 22,
		/* Trying to read data from a unbuffered read buffer.
		*/
		READ_BUFFER_OVERFLOW = 23,
		/* Write stream failed to write to a block.
		*/
		WRITE_BUFFER_OVERFLOW = 24,
		/* BlockDevice was sent an invalid block for read/writing
		*/
		INVALID_BLOCK = 25,
		/* Buffer was not a read buffer
		*/
		INVALID_READ_BUFFER = 26,
		/* Buffer was not a write buffer
		*/
		INVALID_WRITE_BUFFER = 27
	};

	/* Returns if the file error is good
	*/
	static bool good(FileError err) {
		return err == FileError::SUCCESS;
	}
	/* Returns if the file error is bad
	*/
	static bool bad(FileError err) {
		return err != FileError::SUCCESS;
	}
}