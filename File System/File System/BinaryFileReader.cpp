#include "BinaryFileReader.h"
#include"StringFunction.h"
#include<memory>

namespace mf{

	BinaryFileReader::BinaryFileReader()
		: BinaryReader(_file_stream)
	{
		//Stream reference may not be initialized and should not be used.
	}


	BinaryFileReader::~BinaryFileReader()
	{
		_file_stream.close();
	}


	/* Try to open a file for reading
	directory	<<	File directory & name
	return		>>	True if the file is open
	*/
	bool BinaryFileReader::openFile(const std::string &directory){
		_file_stream.open(directory, std::ios::in | std::ios::binary);
		return _file_stream.is_open();
	}
	bool BinaryFileReader::openFile(const std::wstring &directory) {
		_file_stream.open(directory, std::ios::in | std::ios::binary);
		return _file_stream.is_open();
	}
	bool BinaryFileReader::isOpen(){
		return _file_stream.is_open();
	}

	/* Close the stream
	*/
	void BinaryFileReader::close() {
		_file_stream.close();
	}

	
}
