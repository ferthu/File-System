#pragma once

#include<string>

namespace file {

	/* Reference to a file stored in a memory block
	*/
	struct FileReference {
		/* Name of the file.
		*/
		std::string _name;
		/* Block the file header is stored.
		*/
		int	_block;
		/* Byte size of the file
		*/
		unsigned int _byte_size;
		/* Construct a empty reference
		*/
		FileReference()
			: _name(), _block(-1), _byte_size(0) {		}
		/* Construct a file reference from a name, the indexed block where file header is stored.
		name		<<	Name of the file.
		block		<<	The block index where the file header is stored
		byte_size	<<	The size of the file data in bytes.
		*/
		FileReference(std::string name, int block, unsigned int byte_size)
		: _name(name), _block(block), _byte_size(byte_size) {		}

		/* Compare the name to the file name
		*/
		bool operator==(const std::string& compare) {
			return _name == compare;
		}
	};

}