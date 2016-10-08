#pragma once

#include"memblockdevice.h"
#include"FileError.h"
#include"FileReference.h"
#include<string>
#include<memory>
#include"IBlockOwner.h"
#include"File.h"
#include"BinaryFileReader.h"
#include"BinaryFileWriter.h"


namespace file {

	/* Block manager takes care of the entire management of the data stored on the virtual disk.
	*/
	class BlockManager
	{
	private:
		/* The virtual disk
		*/
		MemBlockDevice _disk;
		/* Block managing allocating/releasing blocks.
		*/
		std::unique_ptr<IBlockOwner> _owner;
		/* Size of each block
		*/
		const unsigned int _block_size = 512;

		/* Get the number of blocks required to hold a set of bytes
		*/
		unsigned int numBlocks(size_t bytes);
		/* Removes a block identifier from the vector and returns it. 
		*/
		int extractHeadBlock(std::vector<int>& blocks);

		/* Writes a file object, changing the input variables ONLY if file was written successfully
		file		<>	File information except the allocated blocks to be written. Blocks is assigned on success, previous blocks is ensured to be intact on failure.
		created_ref	<>	File reference, receives the file if write was successfull. Ensured to be intact on failure.
		*/
		err::FileError writeFile(File& file, FileReference& created_ref);
		/* Overwrites a file reference while ensuring old file is intact if an error occured. Releases any extra blocks contained in file on success.  Same guarantee as writeFile(...).
		file		<>	File information except the allocated blocks to be written. Blocks is assigned on success, previous blocks is ensured to be intact on failure.
		created_ref	<>	File reference, receives the file if write was successfull. Ensured to be intact on failure.
		*/
		err::FileError overwriteFile(File& file, FileReference& file_ref);

#pragma region Deprecated: No incrementing! New write preserves state on failure!

		/* Release the allocated blocks in the vector until it is the size of numblocks
		*/
		void releaseExtra(int numblocks, std::vector<int>& blocks);
		/* Allocate the extra blocks so the vector is the size of numblocks
		*/
		err::FileError allocateMore(int numblocks, std::vector<int>& blocks);

#pragma endregion

		BlockManager(const BlockManager& manager) = delete;
		BlockManager& operator=(const BlockManager& manager) = delete;
	public:


		BlockManager(unsigned int num_blocks = 512);
		BlockManager(BlockManager&& manager);

		virtual ~BlockManager();
		/*	Write a file 
		name		<<	Name of the file
		data		<<	Data input for the file
		created_ref	>>	Reference to the file created 
		return		>>	Returns if the file was created or if an error occured.
		*/
		err::FileError writeFile(const std::string& name, const std::string& data, FileReference& created_ref);
		/* Copy the specific file
		name		<<	Name of the new file.
		from		<<	Reference to the file that is to be copied.
		created_ref	>>	Reference to the created file.
		return		>>	Returns if a copy of the file was created or if an error occured.
		*/
		err::FileError copyFile(const std::string& name, const FileReference& from, FileReference& created_ref);
		/* Overwrite the data in the file
		file_to_edit	<<	Reference to the file that is overwritten
		data			<<	Data to write to the file
		return			>>	Returns if the data was successfully written to the file or if an error occured.
		*/
		err::FileError overwriteFile(FileReference& file_to_edit, const std::string& data);
		/*	Read the data from a specified file
		file	<<	Reference to the file to read from
		data	>>	Data read from the file
		return	>>	Returns if data was successfully read from the file
		*/
		err::FileError readFile(const FileReference& file, std::string& data);
		/* Appends from the first file to the other removing the first file.
		from	<<	File to append to the other and remove when operation is complete
		to		<<	Second file that the first file is appended to, reference remains constant
		return	>>	If the data contained in the file was successfully appended.
		*/
		err::FileError appendFile(FileReference& from, FileReference& to);

		/* Remove any data related to the file reference. Any data will be removed regardless if all information exists or not.
		file	<<	File reference to be removed
		return	>>	 Verifies file is writable or reference pointed to a header. If another problem was found with the file reference, any relevant data is removed even on failure.
		*/
		err::FileError removeFile(const FileReference& file);


		/* Write to stream
		*/
		void writeToStream(mf::BinaryFileWriter& writer);
		/* Read from stream.
		*/
		static BlockManager readFromStream(mf::BinaryFileReader& reader);


	};

}
