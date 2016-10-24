#include"FileError.h"

namespace err {

	/* Get a error message
	*/
	const std::string& getMsg(FileError error) {
		const static std::string errors[28]{
			"Success!",
			"DIRECTORY_DOES_NOT_EXIST - A specified directory did not exist at the directory level.",
			"FOLDER_DOES_NOT_EXIST - A specified folder is called for removal but it did not exist.",
			"FILE_NOT_FOUND - A specified file is not found in a directory.",
			"FILE_ALREADY_EXIST - New file tried to be created with the same name as an existing file.",
			"CORRUPTED_FILE - A non-existing file reference was found in the file system.",
			"MEMORY_OVERFLOW - Not enough available memory in file system.",
			"CORRUPTED_BLOCK - A bad block was sent for release to memory manager.",
			"NOT_FOUND - File or folder was not found",
			"TO_LARGE_HEADER - File header was to large to be stored in system.",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"STREAM_CLOSED",
			"",
			"NO_READ_ACCESS - Not allowed to read the file.",
			"NO_WRITE_ACCESS - Not allowed to write to the file.",
			"READ_BUFFER_UNDERFLOW - Trying to read data from a unbuffered read buffer.",
			"WRITE_BUFFER_OVERFLOW - Write stream failed to write to a block.",
			"INVALID_BLOCK -  BlockDevice was sent an invalid block for read/writing",
			"INVALID_READ_BUFFER - Buffer was not a read buffer.",
			"INVALID_WRITE_BUFFER - Buffer was not a write buffer."
		};
		return errors[abs(error)];
	}
}