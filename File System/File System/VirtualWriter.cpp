#include "VirtualWriter.h"

namespace file {

	VirtualWriter::VirtualWriter(BlockDevice &device)
		: _streambuf(device), _stream(&_streambuf), _writer(_stream)
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
		//Verify buffer size
		if (header.headerByteSize() > _streambuf.getBlockSize())
			return err::TO_LARGE_HEADER;

		_writer.writeChar(header._access);
		_writer.writeString(header._fileName);
		_writer.writeUInt(header._size);
		_writer.writeVector<int>(header._blocks);
		return _streambuf.close();
	}

	/* Read a file
	*/
	err::FileError VirtualWriter::writeFileData(const FileHeader& header, const char* file_data) {
		err::FileError error = _streambuf.openWrite(header._blocks);
		if (err::bad(error))
			return error;
		//Write file data
		_writer.writeArray(file_data, header._size);
		return _streambuf.close();
	}
	err::FileError VirtualWriter::writeFile(int block, const File& file) {
		err::FileError error = writeHeader(block, file._header);
		if (err::bad(error))
			return error;
		return writeFileData(file._header, file._data.get());
	}
}
