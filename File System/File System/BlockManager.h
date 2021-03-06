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
		/* Release blocks allocated for a file. Checks if the parsed blocks are corrupted before removing.
		header			<<	File header containing the blocks to release and byte size.
		header_block	<<	Block file header is stored in.
		*/
		err::FileError releaseFileBlocks(const FileHeader& header, const int header_block);
		
		BlockManager(const BlockManager& manager) = delete;
		BlockManager& operator=(const BlockManager& manager) = delete;
	public:

		BlockManager(unsigned int num_blocks = 512);
		BlockManager(BlockManager&& manager);

		virtual ~BlockManager();
		/*	Write a file 
		name		<<	Name of the file
		data		<<	Data input for the file
		access		<<	Access rights for the created file
		created_ref	>>	Reference to the file created 
		return		>>	Returns if the file was created or if an error occured.
		*/
		err::FileError writeFile(const std::string& name, const std::string& data, char access, FileReference& created_ref);
		/* Copy the specific file
		name		<<	Name of the new file.
		from		<<	Reference to the file that is to be copied.
		copy_ref	<>	Reference to existing file to overwrite (if one exists). Returns the copy file reference.
		return		>>	Returns if a copy of the file was created or if an error occured.
		*/
		err::FileError copyFile(const std::string& name, const FileReference& from, FileReference& copy_ref);
		/* Overwrite the data in the file
		file_to_edit	<<	Reference to the file that is overwritten
		data			<<	Data to write to the file
		access			<<	Access of the file created
		return			>>	Returns if the data was successfully written to the file or if an error occured.
		*/
		err::FileError overwriteFile(FileReference& file_to_edit, const std::string& data, char access);
		/*	Read the data from a specified file
		file	<<	Reference to the file to read from
		data	>>	Data read from the file
		return	>>	Returns if data was successfully read from the file
		*/
		err::FileError readFile(const FileReference& file, std::string& data);
		/* Appends from the first file to the other removing the first file.
		from	<<	File to append to the other and remove when operation is complete
		to		<>	Second file that the first file is appended to, reference file is overwritten returning a new reference
		return	>>	If the data contained in the file was successfully appended.
		*/
		err::FileError appendFile(const FileReference& from, FileReference& to);

		/* Rewrite the file header with the specified access status
		*/
		err::FileError writeAccess(const FileReference& file, char access);

		/* Remove any data related to the file reference. Any data will be removed regardless if all information exists or not.
		file	<<	File reference to be removed
		return	>>	 Verifies file is writable or reference pointed to a header. If another problem was found with the file reference, any relevant data is removed even on failure.
		*/
		err::FileError removeFile(const FileReference& file);

		/* Verifies that the specified file has the access.
		file	<<	File to verify
		access	<<	Access that the file should have.
		return	>>	Error if a specified access is not met or if file does not exist.
		*/
		err::FileError verifyAccess(const FileReference& file, char access);

		/* Get the total number of blocks in manager.
		*/
		unsigned int blockSize();

		/* Write to stream
		*/
		void writeToStream(mf::BinaryFileWriter& writer);
		/* Read from stream.
		*/
		static BlockManager readFromStream(mf::BinaryFileReader& reader);

		/* Resets blocks
		*/
		void format();
	};

}
