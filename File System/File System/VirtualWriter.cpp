#include "VirtualWriter.h"

namespace file {

	VirtualWriter::VirtualWriter(BlockDevice &device)
		: mf::BinaryWriter(_stream), _streambuf(device), _stream(&_streambuf)
	{
	}


	VirtualWriter::~VirtualWriter()
	{
		_streambuf.close();
	}
	err::FileError VirtualWriter::getErr() {
		return _streambuf.getErr();
	}

	
	/* Read a file header
	*/
	err::FileError VirtualWriter::writeHeader(int block, const FileHeader& header) {
		err::FileError error = _streambuf.openWrite(block);
		if (!err::good(error))
			return error;

		writeChar(header._access);
		writeString(header._fileName);
		writeUInt(header._size);
		writeVector(header._blocks);
		return _streambuf.getErr();
	}

	/* Read a file
	*/
	err::FileError VirtualWriter::writeFileData(const FileHeader& header, const char* file_data) {
		if (!header.isWritable())
			return err::NO_WRITE_ACCESS;
		err::FileError error = _streambuf.openWrite(header._blocks);
		if (!err::good(error))
			return error;
		//Write file data
		writeArray(file_data, header._size);
		return _streambuf.getErr();
	}
}
