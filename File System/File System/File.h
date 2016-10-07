#pragma once

#include<string>
#include<vector>
#include<memory>
#include"FileHeader.h"

namespace file {

	/* The file representation in the tree
	*/
	class File
	{
	public:
		File();
		File(const std::string& name, char access, const std::string& data);
		virtual ~File();

		/* The header info of the file.
		*/
		FileHeader _header;
		/* Data contained in the file
		*/
		std::unique_ptr<char> _data;

		/* Set the data in the file
		*/
		void setData(const std::string& data);

		/* Get data as string
		*/
		std::string getData();

		/* Append the file
		*/
		void append(File& other);
	};

}

