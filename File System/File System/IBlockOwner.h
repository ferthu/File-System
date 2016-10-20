#pragma once

#include"FileError.h"
#include<vector>

namespace file {

	/* Interface for managing allocating and releasing memory blocks in the system. 
	*/
	class IBlockOwner {
	public:

		/* Allocate a free block.
		block_num	>>	The block allocated
		return		>>	Success if there is a free block available.
		*/
		virtual err::FileError allocate(int& block_num) = 0;
		/* Allocate a number of free blocks.
		num			<<	The number of blocks to allocate.
		block_num	>>	The identities of the blocks allocated.
		return		>>	Success if there was enough blocks available.
		*/
		virtual err::FileError allocate(unsigned int num, std::vector<int>& block_num) = 0;

		/* Release a allocated block.
		block_num	>>	The block to release
		return		>>	Success if no error occured on releasing the block (access, invalid block number, already released block)
		*/
		virtual err::FileError release(int block_num) = 0;
		/* Release a set of blocks.
		block_num	>>	The identities of the blocks to release.
		return		>>	Success if no error occured on releasing the blocks (access, invalid block number, already released block)
		*/
		virtual err::FileError release(const std::vector<int>& block_num) = 0;
		
		/*	Get the number of free memory blocks
		*/
		virtual size_t free() = 0;

		/* Get occupied blocks
		*/
		virtual std::vector<int> getOccupied() = 0;
		/* Occupy a specified set of blocks may change the forwarded list. Formats owner if not empty.
		*/
		virtual void occupy(std::vector<int>& occupied) = 0;
		/* Format all blocks releasing the data.
		*/
		virtual void format() = 0;

		/* Virtual destructor*/
		virtual ~IBlockOwner() {};
	};

}