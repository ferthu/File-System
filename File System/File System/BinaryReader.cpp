#include "BinaryReader.h"
#include"StringFunction.h"

namespace mf {

	BinaryReader::BinaryReader(std::istream& stream)
		: _stream(stream)
	{
	}


	BinaryReader::~BinaryReader()
	{
	}

	bool BinaryReader::readBool() {
		bool value;
		_stream.read((char*)(&value), sizeof(bool));
		return value;
	}
	char BinaryReader::readChar() {
		char value;
		_stream.read((char*)(&value), sizeof(char));
		return value;
	}
	int BinaryReader::readInt() {
		int value;
		_stream.read((char*)(&value), sizeof(int));
		return value;

	}
	unsigned int BinaryReader::readUInt() {
		unsigned int value;
		_stream.read((char*)(&value), sizeof(unsigned int));
		return value;
	}
	float BinaryReader::readFloat() {
		float value;
		_stream.read((char*)(&value), sizeof(float));
		return value;

	}
	double BinaryReader::readDouble() {
		double value;
		_stream.read((char*)(&value), sizeof(double));
		return value;

	}
	std::string BinaryReader::readString() {
		unsigned int length = readUInt();
		char *strArray = new char[length + 1];
		_stream.read((char*)(strArray), sizeof(char)* length);
		strArray[length] = STRINGEND;
		std::string str(strArray);
		delete[] strArray;
		return str;
	}
}
