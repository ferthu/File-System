#include "BinaryWriter.h"

namespace mf{


	BinaryWriter::BinaryWriter(std::ostream& stream)
		: _stream(stream)
	{	}


	BinaryWriter::~BinaryWriter()
	{	}

	void BinaryWriter::writeBool(const bool value){
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(bool));
	}
	void BinaryWriter::writeChar(const char value){
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(char));
	}
	void BinaryWriter::writeInt(const int value){
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(int));

	}
	void BinaryWriter::writeUInt(const unsigned int value) {
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(unsigned int));
	}
	void BinaryWriter::writeFloat(const float value){
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(float));

	}
	void BinaryWriter::writeDouble(const double value){
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(double));

	}
	void BinaryWriter::writeString(const std::string &value){
		writeUInt((unsigned int)value.length());
		_stream.write(value.c_str(), sizeof(char)* value.length());
	}


}