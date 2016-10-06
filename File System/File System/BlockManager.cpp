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
	int  BlockManager::numBlocks(size_t bytes) {
		return (int)(bytes / _block_size + (bytes % (bytes + 1) > 0));
	}
	/* Get the number of blocks required to hold a file
	*/
	int BlockManager::numBlocks(const FileHeader& file) {
		size_t byte_size =
			file._fileName.length() * sizeof(char)
			+ file._access * sizeof(char)
			+ file._size * sizeof(unsigned int);
		//Check how many blocks the file needs without storing block numbers:
		int blocks = numBlocks(byte_size);
		/*Verify that the same amount of blocks covers the size of the block integers.
		If the block count increases it will fit in the extra block (unless block size is < 4 * 2 bytes...)
		*/
		byte_size += file._blocks.size() * sizeof(int);
		return (int)numBlocks(byte_size);
	}

	/* Helper function to separate a block for the header */
	int extractHeadBlock(std::vector<int>& blocks) {
		//Acquire the first buffer for the header
		int head_block = blocks.begin()[0];
		blocks.erase(blocks.begin());
		return head_block;
	}

	/*	Write a file
	name		<<	Name of the file
	data		<<	Data input for the file
	created_ref	>>	Reference to the file created
	return		>>	Returns if the file was created or if an error occured.
	*/
	err::FileError BlockManager::writeFile(const std::string& name, std::string& data, FileReference& created_ref) {

		File file(name, 0, data);
		std::vector<int>& blocks = file._header._blocks;

		//Allocate blocks for the header
		err::FileError error = _owner->allocate(numBlocks(data.size()) + 1, blocks);
		if (!err::good(error))
			return error;
		//Extract a block for the header
		int head_block = extractHeadBlock(file._header._blocks);

		//Write header
		VirtualWriter writer(_disk);
		error = writer.writeHeader(head_block, file._header);
		if (err::good(error)) {
			//All fine: Write file data
			error = writer.writeFileData(file._header, file._data.get());
			if (err::good(error)) {
				//Set the file reference:
				created_ref._block = head_block;
				created_ref._name = name;
				return err::SUCCESS;
			}
		}
		//Fail: release blocks (data remains but is not attached to the system)
		_owner->release(blocks);
		_owner->release(head_block);
		return error;
	}
	/* Copy the specific file
	name		<<	Name of the new file.
	from		<<	Reference to the file that is to be copied.
	created_ref	>>	Reference to the created file.
	return		>>	Returns if a copy of the file was created or if an error occured.
	*/
	err::FileError BlockManager::copyFile(const std::string& name, const FileReference& from, FileReference& created_ref) {
		return err::NO_READ_ACCESS;
	}
	/* Overwrite the data in the file
	file_to_edit	<<	Reference to the file that is overwritten
	data			<<	Data to write to the file
	return			>>	Returns if the data was successfully written to the file or if an error occured.
	*/
	err::FileError BlockManager::overwriteFile(const FileReference& file_to_edit, const std::string& data) {
		return err::NO_READ_ACCESS;
	}
	/*	Read the data from a specified file
	file	<<	Reference to the file to read from
	data	>>	Data read from the file
	return	>>	Returns if data was successfully read from the file
	*/
	err::FileError BlockManager::readFile(const FileReference& file, std::string& data) {
		return err::NO_READ_ACCESS;
	}
	/* Appends from the first file to the other removing the first file.
	from	<<	File to append to the other and remove when operation is complete
	to		<<	Second file that the first file is appended to, reference remains constant
	return	>>	If the data contained in the file was successfully appended.
	*/
	err::FileError BlockManager::appendFile(const FileReference& from, const FileReference& to) {
		return err::NO_READ_ACCESS;
	}

}