#include "BlockManager.h"
#include"BinaryWriter.h"
#include"VirtualWriter.h"
#include"VirtualReader.h"
#include"SimpleBlockOwner.h"
namespace file {



	BlockManager::BlockManager(unsigned int num_blocks)
		: _disk(num_blocks), _owner(new SimpleBlockOwner(num_blocks))
	{
	}
	BlockManager::BlockManager(BlockManager&& manager)
		: _disk(std::move(manager._disk)), _owner(std::move(manager._owner)) {

	}


	BlockManager::~BlockManager()
	{
	}
	/* Get the total number of blocks in manager.
	*/
	unsigned int BlockManager::blockSize() {
		return _disk.size();
	}
	/* Get the number of blocks required to hold a set of bytes
	*/
	unsigned int  BlockManager::numBlocks(size_t bytes) {
		return (int)(bytes / _block_size + (bytes % (bytes + 1) > 0));
	}

	/* Release blocks allocated for a file. Checks if the parsed blocks are corrupted before removing.
	blocks			<<	File blocks removed
	header_block	<<	
	*/
	err::FileError BlockManager::releaseFileBlocks(const FileHeader& header, const int header_block) {

		//If block size of the file is not equal to the blocks allocated this file is corrupted.
		if (numBlocks(header._size) != header._blocks.size())
			return err::CORRUPTED_FILE;

		//Release blocks in one call
		std::vector<int> rm_blocks(header._blocks.size() + 1);
		unsigned int i = 0;
		for (; i < header._blocks.size(); i++)
			rm_blocks[i] = header._blocks[i];
		rm_blocks[i++] = header_block;
		if (err::bad(_owner->release(rm_blocks)))
			return err::CORRUPTED_BLOCK;
		return err::SUCCESS;
	}

	/* Writes a file object, changing the input variables ONLY if file was written successfully
	file		<>	File information except the allocated blocks to be written. Blocks is assigned on success, previous blocks is ensured to be intact on failure.
	created_ref	<>	File reference, receives the file if write was successfull. Ensured to be intact on failure.
	*/
	err::FileError BlockManager::writeFile(File& file, FileReference& created_ref) {
		std::vector<int> blocks;

		//Allocate blocks for the header
		err::FileError error = _owner->allocate(numBlocks(file._header._size) + 1, blocks);
		if (!err::good(error))
			return error;
		//Extract a block for the header
		int head_block = extractHeadBlock(blocks);
		//Swap in the allocated blocks
		std::swap( blocks, file._header._blocks);

		//Write header
		VirtualWriter writer(_disk);
		error = writer.writeFile(head_block, file);
		if (err::bad(error)) {
			//Swap back any previous block information
			std::swap(file._header._blocks, blocks);
			//Fail: release blocks (data remains but is not attached to the system)
			_owner->release(head_block);
			_owner->release(blocks);
			return error;
		}
		//Set the file reference:
		created_ref = FileReference(file._header._fileName, head_block, file._header._size);
		return err::SUCCESS;
	}
	/* Rewrite the file header with the specified access status
	*/
	err::FileError BlockManager::writeAccess(const FileReference& file, char access) {
		VirtualReader reader(_disk);
		VirtualWriter writer(_disk);
		FileHeader header;

		// read header
		err::FileError err = reader.readHeader(file._block, header);
		if (err::bad(err))
			return err;

		header._access = access;

		// write updated header
		err = writer.writeHeader(file._block, header);
		if (err::bad(err))
			return err;

		return err::SUCCESS;
	}
	/* Overwrites a file reference while ensuring old file is intact if an error occured. Releases any extra blocks contained in file on success.  Same guarantee as writeFile(...).
	file		<>	File information except the allocated blocks to be written. Blocks is assigned on success, previous blocks is ensured to be intact on failure.
	created_ref	<>	File reference, receives the file if write was successfull. Ensured to be intact on failure.
	*/
	err::FileError BlockManager::overwriteFile(File& file, FileReference& file_ref) {
		//Store information of the old file
		int old_block = file_ref._block;
		FileHeader old_head = file._header;
		//Write the new file, ensuring that no state is changed on failure
		err::FileError error = writeFile(file, file_ref);
		//If bad error old file is still intact, return.
		if (err::bad(error))
			return error;
		//Release old file.
		return releaseFileBlocks(old_head, old_block);
	}
	/*	Write a file
	name		<<	Name of the file
	data		<<	Data input for the file
	created_ref	>>	Reference to the file created
	return		>>	Returns if the file was created or if an error occured.
	*/
	err::FileError BlockManager::writeFile(const std::string& name, const std::string& data, char access, FileReference& created_ref) {
		File file(name, access, data);
		return writeFile(file, created_ref);
	}
	/* Remove any data related to the file reference. Any data will be removed regardless if all information exists or not.
	file	<<	File reference to be removed
	return	>>	 Verifies file is writable and readable, or if reference pointed to a valid header. If another problem was found with the file reference, any relevant data is removed even on failure.
	*/
	err::FileError BlockManager::removeFile(const FileReference& file) {
		VirtualReader reader(_disk);
		FileHeader header;
		reader.readHeader(file._block, header);
		if (!header.isWritable())
			return err::NO_WRITE_ACCESS;
		return releaseFileBlocks(header, file._block);
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
	copy_ref	<>	Reference to existing file to overwrite (if one exists). Returns the copy file reference.
	return		>>	Returns if a copy of the file was created or if an error occured.
	*/
	err::FileError BlockManager::copyFile(const std::string& name, const FileReference& from, FileReference& copy_ref) {
		VirtualReader reader(_disk);

		File ret;
		err::FileError error = reader.readFile(from._block, ret);
		if (err::bad(error))
			return error;
		//Change name
		ret._header._fileName = name;
		//Write or overwrite an existing file with the copy.
		if (copy_ref.empty())
			return writeFile(ret, copy_ref);
		else
			return overwriteFile(copy_ref, ret.getData(), ret._header._access);
	}
	/* Overwrite the data in the file
	file_to_edit	<<	Reference to the file that is overwritten
	data			<<	Data to write to the file
	access			<<	Access rights to the created file
	return			>>	Returns if the data was successfully written to the file or if an error occured.
	*/
	err::FileError BlockManager::overwriteFile(FileReference& file_to_edit, const std::string& data, char access) {
		
		VirtualReader reader(_disk);
		File file;
		//Start by reading the old header
		err::FileError error = reader.readHeader(file_to_edit._block, file._header);
		if (err::bad(error))
			return error;
		//Verify file is writable
		if (!file._header.isWritable())
			return err::NO_WRITE_ACCESS;
		//Set new access
		file._header._access = access;
		//Change file data
		file.setData(data);
		//Overwrite the file preserving old file on error
		return overwriteFile(file, file_to_edit);
	}

	/* Appends from the first file to the other removing the first file.
	from	<<	File to append to the other and remove when operation is complete
	to		<>	Second file that the first file is appended to, reference file is overwritten returning a new reference
	return	>>	If the data contained in the file was successfully appended.
	*/
	err::FileError BlockManager::appendFile(const FileReference& from, FileReference& to) {
		VirtualReader reader(_disk);
		//File a is the appended file, b is deleted on success.
		File file_a, file_b;
		//Read the appended file
		err::FileError error = reader.readFile(to._block, file_a);
		if (err::bad(error))
			return error;
		//Verify the appended file is writable
		if (!file_a._header.isWritable())
			return err::NO_WRITE_ACCESS;

		//Read the file to append
		error = reader.readFile(from._block, file_b);
		if (err::bad(error))
			return error;

		//Create a new file with appended data
		File new_a(file_a, file_b);

		//Overwrite the file preserving old file's on error
		error = overwriteFile(new_a, to);
		if (err::bad(error))
			return error;

		//Release the appended file.
		return releaseFileBlocks(file_b._header, from._block);
	}

	/* Verifies that the specified file has the access.
	file	<<	File to verify
	access	<<	Access that the file should have.
	return	>>	Error if a specified access is not met or if file does not exist.
	*/
	err::FileError BlockManager::verifyAccess(const FileReference& file, char access) {
		VirtualReader reader(_disk);
		FileHeader header;
		err::FileError error = reader.readHeader(file._block, header);
		if (err::bad(error))
			return error;
		//Verify all good
		if (access == header._access || (header._access > 2 && access != 0))
			return err::SUCCESS;
		return access == 1 ? err::NO_READ_ACCESS : err::NO_WRITE_ACCESS;
	}
	/* Write to stream
	*/
	void  BlockManager::writeToStream(mf::BinaryFileWriter& writer) {
		std::vector<int> occupied = _owner->getOccupied();

		//Write Owner....
		writer.writeUInt(blockSize());
		writer.writeUInt((unsigned int)occupied.size());

		for (int i = 0; i < occupied.size(); i++) {
			writer.writeInt(occupied[i]);
			Block& data = _disk[occupied[i]];
			writer.writePtr(data.begin(), data.size());
		}
	}
	/* Read from stream.
	*/
	BlockManager  BlockManager::readFromStream(mf::BinaryFileReader& reader) {

		//Read block size and initiate
		unsigned int block_size = reader.readUInt();
		BlockManager manager(block_size);

		unsigned int num_occupied = reader.readUInt();
		//Occupired blocks
		std::vector<int> occupied(num_occupied);

		for (unsigned int i = 0; i < num_occupied; i++) {
			int block_num = reader.readInt();
			occupied[i] = block_num;
			size_t size;
			char* ptr = reader.readPtr<char>(size);
			manager._disk.writeBlock(block_num, ptr);
			delete[] ptr;
		}
		//Initiate block owner
		manager._owner->occupy(occupied);
		return manager;
	}
	/* Helper function to separate a block for the header */
	int BlockManager::extractHeadBlock(std::vector<int>& blocks) {
		//Acquire the first buffer for the header
		int head_block = blocks.begin()[0];
		blocks.erase(blocks.begin());
		return head_block;
	}

	void BlockManager::format()
	{
		_disk.reset();
		_owner->format();
	}
}

