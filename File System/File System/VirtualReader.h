#pragma once

#include"VirtualStream.h"
#include"BinaryReader.h"
#include"File.h"

namespace file {

	class VirtualReader 
	{
	private:
		/* The custom std::streambuf
		*/
		VirtualStream _streambuf;
		/* The read istream construction
		*/
		std::istream _stream;
		/* The stream reader
 		*/
		mf::BinaryReader _reader;

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
		err::FileError readFileData(const FileHeader& header, std::unique_ptr<char>& file_data);

		/* Read entire file. Verifies it is readable
		*/
		err::FileError readFile(int block, File& file);

	};

}

