#include "VirtualReader.h"


namespace file {


	VirtualReader::VirtualReader(BlockDevice &device)
		: mf::BinaryReader(_stream), _streambuf(device), _stream(&_streambuf)
	{
	}


	VirtualReader::~VirtualReader()
	{
	}

	err::FileError VirtualReader::getErr() {
		return _streambuf.getErr();
	}

	/* Read a file header
	*/
	err::FileError VirtualReader::readHeader(int block, FileHeader& header) {
		err::FileError error = _streambuf.openRead(block);
		if (!err::good(error))
			return error;

		header._access = readChar();
		header._fileName = readString();
		header._size = readUInt();
		header._blocks = readVector<int>();
		return _streambuf.close();
	}

	/* Read a file
	*/
	err::FileError VirtualReader::readFileData(const FileHeader& header, char* file_data) {
		if (!header.isReadable())
			return err::NO_READ_ACCESS;
		err::FileError error = _streambuf.openRead(header._blocks);
		if (!err::good(error))
			return error;
		file_data = readArray<char>(header._size);
		return _streambuf.close();
	}

}
