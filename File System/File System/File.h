#pragma once

#include<string>
#include<vector>
#include"FileHeader.h"

namespace file {

	/* The file representation in the tree
	*/
	class File
	{
	public:
		File();
		virtual ~File();

		/* The header info of the file.
		*/
		FileHeader _header;
		/* Data contained in the file
		*/
		char* _data;
	};

}

