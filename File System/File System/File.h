#pragma once

#include<string>
#include<vector>

namespace file {

	/* The file representation in the tree
	*/
	class File
	{
	public:
		File();
		virtual ~File();
		/* Write/Read access to the file
		*/
		bool _readable, _writable;
		/* Name of the file
		*/
		std::string _file_name;
		/* Size of file in bytes.
		*/
		unsigned int _file_size;
		/* Blocks occupied by the file.
		*/
		std::vector<int> _blocks;
	};

}

