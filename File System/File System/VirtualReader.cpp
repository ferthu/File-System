#include "VirtualReader.h"


namespace file {


	VirtualReader::VirtualReader(BlockDevice &device)
		:  _streambuf(device), _stream(&_streambuf), _reader(_stream)
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

		header._access = _reader.readChar();
		header._fileName = _reader.readString();
		header._size = _reader.readUInt();
		header._blocks = _reader.readVector<int>();
		return _streambuf.close();
	}

	/* Read a file
	*/
	err::FileError VirtualReader::readFileData(const FileHeader& header, std::unique_ptr<char>& file_data) {
		err::FileError error = _streambuf.openRead(header._blocks);
		if (!err::good(error))
			return error;
		//Read data
		file_data = std::unique_ptr<char>(_reader.readArray<char>(header._size));
		return _streambuf.close();
	}

	/* Read entire file. Verifies it is readable
	*/
	err::FileError VirtualReader::readFile(int block, File& file) {
		//Read header
		err::FileError error = readHeader(block, file._header);
		if (!err::good(error))
			return error;
		if (!file._header.isReadable())
			return err::NO_READ_ACCESS;
		//Read file
		return readFileData(file._header, file._data);
	}
}
