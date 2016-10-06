#include "BinaryFileWriter.h"

namespace mf {


	BinaryFileWriter::BinaryFileWriter()
		: BinaryWriter(_file_stream)
	{
	}


	BinaryFileWriter::~BinaryFileWriter()
	{
		_file_stream.close();
	}
	bool BinaryFileWriter::openFile(const std::string &directory) {
		_file_stream.open(directory, std::ios::out | std::ios::binary | std::ios::trunc);
		return _file_stream.is_open();
	}
	bool BinaryFileWriter::openFile(const std::wstring &directory) {
		_file_stream.open(directory, std::ios::out | std::ios::binary | std::ios::trunc);
		return _file_stream.is_open();
	}
	bool BinaryFileWriter::isOpen() {
		return _file_stream.is_open();
	}

	/* Close the stream
	*/
	void BinaryFileWriter::close() {
		_file_stream.close();
	}

}