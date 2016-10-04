#pragma once

#include<string>
#include<vector>

namespace file {
	/* Header information of a file
	*/
	class FileHeader {

		/* Write/Read access to the file
		*/
		bool _readable, _writable;
		/* Name of the file
		*/
		std::string _fileName;
		/* Blocks occupied by the file.
		*/
		std::vector<int> _blocks;
	};

}