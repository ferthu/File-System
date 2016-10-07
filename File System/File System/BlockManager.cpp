#include "BlockManager.h"
#include"BinaryWriter.h"
#include"VirtualWriter.h"
#include"VirtualReader.h"

namespace file {



	BlockManager::BlockManager(unsigned int num_blocks)
		: _disk(num_blocks)
	{
	}


	BlockManager::~BlockManager()
	{
	}
	/* Get the number of blocks required to hold a set of bytes
	*/
	unsigned int  BlockManager::numBlocks(size_t bytes) {
		return (int)(bytes / _block_size + (bytes % (bytes + 1) > 0));
	}
	/* Release the extra blocks so the vector is of size numblocks
	*/
	void BlockManager::releaseExtra(int numblocks, std::vector<int>& blocks) {
		//Release the extra blocks
		for (unsigned int i = numblocks; i < blocks.size(); i++)
			_owner->release(blocks[i]);
		//Erase the blocks from the file
		blocks.erase(blocks.begin() + numblocks, blocks.end());
	}
	/* Allocate the extra blocks so the vector is the size of numblocks
	*/
	err::FileError  BlockManager::allocateMore(int numblocks, std::vector<int>& blocks) {
		std::vector<int> new_blocks;
		err::FileError error = _owner->allocate(numblocks - blocks.size(), new_blocks);
		//Verify no error allocating blocks
		if (err::bad(error))
			return error;
		//Add blocks to the block vector
		for (unsigned int i = 0; i < new_blocks.size(); i++)
			blocks.push_back(new_blocks[i]);
		return err::SUCCESS;
	}
	/* Helper function to separate a block for the header */
	int extractHeadBlock(std::vector<int>& blocks) {
		//Acquire the first buffer for the header
		int head_block = blocks.begin()[0];
		blocks.erase(blocks.begin());
		return head_block;
	}

	/* Writes a file object
	*/
	err::FileError BlockManager::writeFile(File& file, FileReference& created_ref) {
		std::vector<int>& blocks = file._header._blocks;

		//Allocate blocks for the header
		err::FileError error = _owner->allocate(numBlocks(file._header._size) + 1, blocks);
		if (!err::good(error))
			return error;
		//Extract a block for the header
		int head_block = extractHeadBlock(file._header._blocks);

		//Write header
		VirtualWriter writer(_disk);
		error = writer.writeFile(head_block, file);
		if (err::bad(error)) {
			//Fail: release blocks (data remains but is not attached to the system)
			_owner->release(blocks);
			_owner->release(head_block);
			return error;
		}
		//Set the file reference:
		created_ref._block = head_block;
		created_ref._name = file._header._fileName;
		return err::SUCCESS;
	}
	/*	Write a file
	name		<<	Name of the file
	data		<<	Data input for the file
	created_ref	>>	Reference to the file created
	return		>>	Returns if the file was created or if an error occured.
	*/
	err::FileError BlockManager::writeFile(const std::string& name, std::string& data, FileReference& created_ref) {
		File file(name, 0, data);
		return writeFile(file, created_ref);
	}
	/*	Read the data from a specified file
	file	<<	Reference to the file to read from
	data	>>	Data read from the file
	return	>>	Returns if data was successfully read from the file
	*/
	err::FileError BlockManager::readFile(const FileReference& file, std::string& data) {
		VirtualReader reader(_disk);

		File ret;
		err::FileError error = reader.readFile(file._block, ret);
		if (err::bad(error))
			return error;
		
		//Extract data
		data = ret.getData();

		return err::SUCCESS;
	}
	/* Copy the specific file
	name		<<	Name of the new file.
	from		<<	Reference to the file that is to be copied.
	created_ref	>>	Reference to the created file.
	return		>>	Returns if a copy of the file was created or if an error occured.
	*/
	err::FileError BlockManager::copyFile(const std::string& name, const FileReference& from, FileReference& created_ref) {
		VirtualReader reader(_disk);

		File ret;
		err::FileError error = reader.readFile(from._block, ret);
		if (err::bad(error))
			return error;
		//Change name
		ret._header._fileName = name;
		//Write a copy
		return writeFile(ret, created_ref);
	}
	/* Overwrite the data in the file
	file_to_edit	<<	Reference to the file that is overwritten
	data			<<	Data to write to the file
	return			>>	Returns if the data was successfully written to the file or if an error occured.
	*/
	err::FileError BlockManager::overwriteFile(const FileReference& file_to_edit, const std::string& data) {
		
		VirtualReader reader(_disk);
		File file;
		//Start by reading the old header
		err::FileError error = reader.readHeader(file_to_edit._block, file._header);
		if (err::bad(error))
			return error;
		//Verify file is writable
		if (!file._header.isWritable())
			return err::NO_WRITE_ACCESS;

		//Change file data
		file.setData(data);

		//Release or Allocate more blocks if needed
		unsigned int numblocks = numBlocks(file._header._size);
		//Release the extra blocks
		if (file._header._blocks.size() > numblocks)
			releaseExtra(numblocks, file._header._blocks);
		//Allocate more blocks
		else if (file._header._blocks.size() < numblocks) {
			error = allocateMore(numblocks, file._header._blocks);
			if (err::bad(error))
				return error;
		}

		//Overwrite the file
		VirtualWriter writer(_disk);
		return writer.writeFile(file_to_edit._block, file);
	}
	/* Appends from the first file to the other removing the first file.
	from	<<	File to append to the other and remove when operation is complete
	to		<<	Second file that the first file is appended to, reference remains constant
	return	>>	If the data contained in the file was successfully appended.
	*/
	err::FileError BlockManager::appendFile(const FileReference& from, const FileReference& to) {
		VirtualReader reader(_disk);
		File file_a, file_b;
		//Read the appended file
		err::FileError error = reader.readFile(to._block, file_a);
		if (err::bad(error))
			return error;
		//Verify the appended file is writable
		if (file_a._header.isWritable())
			return err::NO_WRITE_ACCESS;

		//Read the file to append
		error = reader.readFile(from._block, file_b);
		if (err::bad(error))
			return error;

		//Append file
		file_a.append(file_b);

		//Release extra blocks
		unsigned int req_blocks = numBlocks(file_a._header._size);
		releaseExtra(req_blocks, file_a._header._blocks);

		//Release the header block for file_b
		_owner->release(from._block);

		//Write the file
		VirtualWriter writer(_disk);
		return writer.writeFile(to._block, file_a);
	}

}