#pragma once
#include "IBlockOwner.h"
#include<vector>

namespace file {

	class SimpleBlockOwner :
		public IBlockOwner
	{
	private:

		/* Status of all blocks (true: occupied, false: free)
		*/
		std::vector<bool> _status;
		/* List of available blocks
		*/
		std::vector<int> _free;
		/* Verify that the specified count is available
		*/
		bool verifyFreeCount(int num);
		/* Verify that a block number can be released
		*/
		bool verifyGoodAllocation(int block_num);
		/* Function used for all allocation. Allocates a free block.
		*/
		int singleAllocate();
		/* Function used for all block releases. Releases a single block.
		*/
		void singleRelease(int block_num);
	public:
		SimpleBlockOwner(int count);
		SimpleBlockOwner(int count, const std::vector<int>& occupied);
		virtual ~SimpleBlockOwner();


		/* Allocate a free block.
		block_num	>>	The block allocated
		return		>>	Success if there is a free block available.
		*/
		virtual err::FileError allocate(int& block_num);
		/* Allocate a number of free blocks.
		num			<<	The number of blocks to allocate.
		block_num	>>	The identities of the blocks allocated.
		return		>>	Success if there was enough blocks available.
		*/
		virtual err::FileError allocate(unsigned int num, std::vector<int>& block_num);

		/* Release a allocated block.
		block_num	>>	The block to release
		return		>>	Success if no error occured on releasing the block (access, invalid block number, already released block)
		*/
		virtual err::FileError release(int block_num);
		/* Release a set of blocks.
		block_num	>>	The identities of the blocks to release.
		return		>>	Success if no error occured on releasing the blocks (access, invalid block number, already released block)
		*/
		virtual err::FileError release(const std::vector<int>& block_num);

		/* Get occupied blocks
		*/
		virtual std::vector<int> getOccupied();
		/* Occupy a specified set of blocks may change the forwarded list. Formats owner if not empty.
		*/
		virtual void occupy(std::vector<int>& occupied);
		/* Format all blocks releasing the data.
		*/
		virtual void format();
		/*	Get the number of free memory blocks
		*/
		virtual size_t free();
	};

}

