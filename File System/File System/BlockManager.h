#pragma once

#include"memblockdevice.h"
#include"FileError.h"
#include"FileReference.h"
#include<string>
#include<memory>
#include"IBlockOwner.h"
#include"File.h"
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
		int numBlocks(size_t bytes);
		/* Get the number of blocks required to hold a file
		*/
		int numBlocks(const FileHeader& file);

	public:


		BlockManager(unsigned int num_blocks = 512);

		/*	Write a file 
		name		<<	Name of the file
		data		<<	Data input for the file
		created_ref	>>	Reference to the file created 
		return		>>	Returns if the file was created or if an error occured.
		*/
		err::FileError writeFile(const std::string& name, std::string& data, FileReference& created_ref);
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
		err::FileError overwriteFile(const FileReference& file_to_edit, const std::string& data);
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
		err::FileError appendFile(const FileReference& from, const FileReference& to);


		virtual ~BlockManager();
	};

}
