#pragma once

#include"VirtualStream.h"
#include"BinaryWriter.h"
#include"File.h"


namespace file {

	class VirtualWriter
	{
	public:
		/* The custom std::streambuf
		*/
		VirtualStream _streambuf;
		/* The read ostream construction
		*/
		std::ostream _stream;
		/* The object for writing the data
		*/
		mf::BinaryWriter _writer;


	public:
		VirtualWriter(BlockDevice &device);
		virtual ~VirtualWriter();
		err::FileError getErr();


		/* Write a file header
		*/
		err::FileError writeHeader(int block, const FileHeader& header);

		/* Write file data
		*/
		err::FileError writeFileData(const FileHeader& header, const char* file_data);
		/* Write file
		block	<<	Block for header data
		file	<<	Header data to write
		return	>>	Returns if any error occured.
		*/
		err::FileError writeFile(int block, const File& file);
	};

}

