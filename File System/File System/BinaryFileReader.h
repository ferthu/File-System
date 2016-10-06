#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include"BinaryReader.h"

namespace mf{

	/* Stream reader for binary files. Combination of the BinaryReader and a file stream.
	*/
	class BinaryFileReader 
		: public BinaryReader
	{
	private:
		/* The file stream that is operated on. Opened/closed on function calls
		*/
		std::ifstream _file_stream;
	public:
		BinaryFileReader();
		~BinaryFileReader();

		/* Try to open a file for reading
		directory	<<	File directory & name
		return		>>	True if the file is open
		*/
		bool openFile(const std::string &directory);
		/* Try to open a file for reading
		directory	<<	File directory & name
		return		>>	True if the file is open
		*/
		bool openFile(const std::wstring &directory);
		/* Verify that the stream is open
		*/
		bool isOpen();

		/* Close the stream
		*/
		void close();

	};
}

