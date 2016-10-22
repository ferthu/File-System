#include "File.h"

namespace file {

	File::File()
	{
	}

	File::File(const std::string& name, char access, const std::string& data)
	: _header(name, access, (unsigned int)data.size()) , _data() {
		setData(data);
	}
	File::File(const File& file, const File& append)
		: _header(file._header._fileName, file._header._access, file._header._size + append._header._size), _data() {
		//Copy data
		char* data = new char[_header._size];
		std::memcpy(data, file._data.get(), file._header._size); //Copy this
		std::memcpy(data + file._header._size, append._data.get(), append._header._size); //Append other
		//Set data
		_data = std::unique_ptr<char>(data);
	}

	File::~File()
	{	}

	/* Set the data in the file
	*/
	void File::setData(const std::string& data) {
		_header._size = (unsigned int)data.size();
		_data = std::unique_ptr<char>(new char[data.size()]);
		std::memcpy(_data.get(), data.data(), data.size());
	}

	/* Get data as string
	*/
	std::string File::getData() {
		return std::string(_data.get(), _header._size);
	}


	/* Append the file
	*/
	void File::appendData(File& other) {
	}
}