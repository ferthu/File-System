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
		/* Construct a empty reference
		*/
		FileReference()
			: _name(), _block(-1) {		}
		/* Construct a file reference from a name and the indexed block where file header is stored.
		*/
		FileReference(std::string name, int block) 
		: _name(name), _block(block){		}

		/* Compare the name to the file name
		*/
		bool operator==(const std::string& compare) {
			return _name == compare;
		}
	};

}