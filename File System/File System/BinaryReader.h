#pragma once

#include<string>
#include<vector>
#include<istream>

namespace mf {

	/* Stream reader for binary data. Consists of a set of functions that is paired with the opposite BinaryWriter class.
	Opperates on a stream reference that is not owned by the object. See BinaryFileReader for operations over file objects. 
	*/
	class BinaryReader
	{
	private:
		/* The stream reference
		*/
		std::istream& _stream;
	public:
		/* Construct it from a reference to an input stream that the reader will operate on.
		*/
		BinaryReader(std::istream& stream);
		virtual ~BinaryReader();

		/* Read a boolean element */
		bool readBool();
		/* Read a single char */
		char readChar();
		/* Read a signed int*/
		int readInt();
		/* Read a unsigned int*/
		unsigned int readUInt();
		float readFloat();
		/* Read a double floating point value */
		double readDouble();
		/* Read a string of characters written to the stream */
		std::string readString();

		/*Read a generic object and return it, only works for simple types containing number variables. Does not work with dynamic strings/arrays
		Element << Value that is read and returned*/
		template <class Element>
		Element read();
		/*Read a generic object to the reference parameter, only works for simple types containing value variables. */
		template <class Element>
		void read(Element &readToReference);
		/* Read an array of data of known length */
		template <class Element>
		Element* readArray(unsigned int length);
		/* Read a vector of data. Returning the data in a vector */
		template <class Element>
		std::vector<Element> readVector();
		/* Read a set of bytes from the stream
		byte_size	>>	Number of bytes read from the stream.
		return		>>	Data pointer read from the stream.
		*/
		template <class Element>
		Element* readPtr(size_t& byte_size);

	};
	template <class Element>
	Element BinaryReader::read() {
		Element e;
		_stream.read((char*)(&e), sizeof(Element));
		return e;
	}
	template <class Element>
	void BinaryReader::read(Element &reference) {
		_stream.read((char*)(&reference), sizeof(Element));
	}
	template <class Element>
	Element* BinaryReader::readArray(unsigned int length) {
		size_t byte_size = length * sizeof(Element);
		std::unique_ptr<char> ptr(new char[byte_size]);
		_stream.read(ptr.get(), byte_size);
		return reinterpret_cast<Element*>(ptr.release());
	}
	/* Read a vector of data. Returning the data in a vector */
	template <class Element>
	std::vector<Element> BinaryReader::readVector() {
		unsigned int size = readUInt();
		std::vector<Element> vec(size);
		_stream.read(reinterpret_cast<char*>(&vec[0]), sizeof(Element) * vec.size());
		return vec;
	}
	/* Read a set of bytes from the stream
	byte_size	>>	Number of bytes read from the stream.
	return		>>	Data pointer read from the stream.
	*/
	template <class Element>
	Element* BinaryReader::readPtr(size_t& byte_size) {
		byte_size = readUInt();
		std::unique_ptr<char> ptr(new char[byte_size]);
		_stream.read(ptr.get(), byte_size);
		return reinterpret_cast<Element*>(ptr.release());
	}

}
