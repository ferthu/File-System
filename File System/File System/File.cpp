#include "File.h"

namespace file {

	File::File()
	{
	}

	File::File(const std::string& name, char access, const std::string& data)
	: _header(name, access, data.size()) , _data(new char[data.size()]) {
		std::memcpy(_data.get(), data.data(), data.size());
	}

	File::~File()
	{	}


}