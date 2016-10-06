#pragma once

#include<string>
#include<vector>

namespace file {
	/* Header information of a file
	*/
	class FileHeader {
	public:

		FileHeader() 
		: _fileName(), _access(), _size() {		}
		FileHeader(const std::string& name, unsigned char access, unsigned int size)
			: _fileName(name), _access(access), _size(size) {		}

		/* Write/Read access to the file
		*/
		char _access;
		/* Name of the file
		*/
		std::string _fileName;
		/* Size of the file
		*/
		unsigned int _size;
		/* Blocks occupied by the file.
		*/
		std::vector<int> _blocks;

		/* Verify the readable access */
		bool isReadable() const {
			return true;
		}
		/* Verify the readable access */
		bool isWritable() const {
			return true;
		}
		/* Verify this is a header */
		bool isValidHeader() const {
			return true;
		}
	};

}