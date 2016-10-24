#pragma once

#include<string>
#include<vector>

namespace file {
	/* Header information of a file
	*/
	class FileHeader {
	public:

		FileHeader() 
		: _fileName(), _access(4), _size() {		}
		FileHeader(const std::string& name, unsigned char access, unsigned int size)
			: _fileName(name), _access(access), _size(size) {		}

		/* Write/Read access to the file
		0: No access
		1: Write Only
		2: Read Only
		2<: Full access
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
			return _access > 1;
		}
		/* Verify the readable access */
		bool isWritable() const {
			return _access == 1 || _access > 2;
		}
		/* Function used to verify header size */
		unsigned int headerByteSize() const {
			return 1 + _fileName.length() + (3 + _blocks.size()) * sizeof(int);
		}
	};

}