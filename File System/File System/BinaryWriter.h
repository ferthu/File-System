#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

namespace mf{

	/* Stream writer for binary data. Consists of a set of functions that is paired with the opposite BinaryReader class.
	Opperates on a stream reference that is not owned by the object. See BinaryFileWriter for operations over file objects.
	*/
	class BinaryWriter
	{
	private:
		/* Output stream reference */
		std::ostream& _stream;
	public:
		/* Construct the writer from a reference to an output stream.
		*/
		BinaryWriter(std::ostream& stream);
		~BinaryWriter();
		
		/* Write a boolean to the stream */
		void writeBool(const bool value);
		/* Write a single character to the stream */
		void writeChar(const char value);
		/* Write a signed int to the stream */
		void writeInt(const int value);
		/* Write a unsigned int to the stream */
		void writeUInt(const unsigned int value);
		/* Write a single floating point value to the stream */
		void writeFloat(const float value);
		/* Write a double floating point value to the stream */
		void writeDouble(const double value);
		/* Write a string to the stream */
		void writeString(const std::string &str);

		/*Write a generic object to the reference parameter, only works for simple types containing value variables */
		template <class Element>
		void write(const Element &value);
		/* Write a generic data array of static length */
		template <class Element>
		void writeArray(const Element *value, int arraySize);
		/* Write a vector of data */
		template <class Element>
		void writeVector(const std::vector<Element>& vector);
		/* Write a pointer of defined byte size containing an array of data.
		pointer	<<	pointer to the data
		size	<<	Size in bytes to write to the stream.
		*/
		template <class Element>
		void writePtr(const Element* pointer, size_t size);
	};
	template <class Element>
	void BinaryWriter::write(const Element &value) {
		_stream.write(reinterpret_cast<const char*>(&value), sizeof(Element));
	}
	template <class Element>
	void BinaryWriter::writeArray(const Element *value, int arraySize) {
		_stream.write(reinterpret_cast<const char*>(value), arraySize);
	}

	/* Write a vector of data */
	template <class Element>
	void BinaryWriter::writeVector(const std::vector<Element>& vector) {
		writePtr(&vector[0], vector.size());
	}
	/* Write a pointer of defined byte size containing an array of data.
	pointer	<<	pointer to the data
	size	<<	Size in bytes to write to the stream.
	*/
	template <class Element>
	void BinaryWriter::writePtr(const Element* value, size_t size) {
		writeUInt((unsigned int)size);
		_stream.write(reinterpret_cast<const char*>(value), size * sizeof(Element));
	}
}

