#pragma once

#include"filesystem.h"

namespace file {

	/*	Object used as interface between the program and the file system. Containing the current working directory.
	*/
	class FileSystemHandle
	{
	public:
		FileSystemHandle();
		~FileSystemHandle();
	};

}

