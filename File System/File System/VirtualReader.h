#pragma once

#include"VirtualStream.h"
#include"BinaryReader.h"
#include"File.h"

namespace file {

	class VirtualReader 
		: protected mf::BinaryReader
	{
	private:
		/* The custom std::streambuf
		*/
		VirtualStream _streambuf;
		/* The read istream construction
		*/
		std::istream _stream;

	public:
		VirtualReader(BlockDevice &device);
		virtual ~VirtualReader();

		/* Get current state of the read buffer.
		*/
		err::FileError getErr();

		/* Read a file header
		*/
		err::FileError readHeader(int block, FileHeader& header);

		/* Read file data part
		*/
		err::FileError readFileData(const FileHeader& header, char* file_data);


	};

}

