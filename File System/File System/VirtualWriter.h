#pragma once

#include"VirtualStream.h"
#include"BinaryWriter.h"
#include"File.h"


namespace file {

	class VirtualWriter
		: protected mf::BinaryWriter
	{
	public:
		/* The custom std::streambuf
		*/
		VirtualStream _streambuf;
		/* The read ostream construction
		*/
		std::ostream _stream;

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
	};

}

