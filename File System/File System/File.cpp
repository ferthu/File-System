#include "File.h"

namespace file {

	File::File()
	{
	}

	File::File(const std::string& name, char access, const std::string& data)
	: _header(name, access, (unsigned int)data.size()) , _data() {
		setData(data);
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
	void File::append(File& other) {
		//Append blocks
		for (unsigned int i = 0; i < other._header._blocks.size(); i++)
			_header._blocks.push_back(other._header._blocks[i]);
		//Append data
		unsigned int newsize = 	_header._size + other._header._size;
		char* data = new char[newsize];
		//Copy this
		std::memcpy(data, _data.get(), _header._size);
		//Append other
		std::memcpy(data + _header._size, other._data.get(), other._header._size);
		//Set
		_data = std::unique_ptr<char>(data);
	}
}