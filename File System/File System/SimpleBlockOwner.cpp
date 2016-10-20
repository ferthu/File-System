#include "SimpleBlockOwner.h"
#include<algorithm>

namespace file {

	SimpleBlockOwner::SimpleBlockOwner(int count)
	 : _status(count, false), _free(count){
		//Add all blocks to the free list.
		for (int i = 0; i < _free.size(); i++)
			_free[i] = i;
	}
	SimpleBlockOwner::SimpleBlockOwner(int count, const std::vector<int>& occupied)
		: _status(count, false), _free() {
		//Occupy the specified blocks.
		for (int i = 0; i < occupied.size(); i++)
			_status[occupied[i]] = true;
		//Reserve free list size
		_free.reserve(count - occupied.size());
		//Add all non-occupied blocks to the free list.
		for (int i = 0; i < _status.size(); i++) {
			if (!_status[i])
				_free.push_back(i);
		}
	}

	SimpleBlockOwner::~SimpleBlockOwner()
	{
	}

	/* Verify that the specified count is available
	*/
	bool SimpleBlockOwner::verifyFreeCount(int num) {
		return _free.size() > num;
	}
	/* Verify that a block number can be released
	*/
	bool SimpleBlockOwner::verifyGoodAllocation(int block_num) {
		return block_num > 0 && block_num < _status.size() && _status[block_num];
	}

	/* Function used for all allocation. Allocates a single block.
	*/
	int SimpleBlockOwner::singleAllocate() {
		//Fetch a free block
		int block_num = _free.back();
		_free.pop_back();
		//Set status
		_status[block_num] = true;
		return block_num;
	}
	/* Function used for all block releases. Releases a single block.
	*/
	void SimpleBlockOwner::singleRelease(int block_num) {
		_status[block_num] = false;
		_free.push_back(block_num);
	}
	/* Allocate a free block.
	block_num	>>	The block allocated
	return		>>	Success if there is a free block available.
	*/
	err::FileError SimpleBlockOwner::allocate(int& block_num) {
		if (verifyFreeCount(1)) {
			block_num = singleAllocate();
			return err::SUCCESS;
		}
		return err::MEMORY_OVERFLOW;
	}
	/* Allocate a number of free blocks.
	num			<<	The number of blocks to allocate.
	block_num	>>	The identities of the blocks allocated.
	return		>>	Success if there was enough blocks available.
	*/
	err::FileError SimpleBlockOwner::allocate(unsigned int num, std::vector<int>& block_num) {
		if (verifyFreeCount(num)) {
			for (int i = 0; i < num; i++)
				block_num.push_back(singleAllocate());
			return err::SUCCESS;
		}
		return err::MEMORY_OVERFLOW;
	}

	/* Release a allocated block.
	block_num	>>	The block to release
	return		>>	Success if no error occured on releasing the block (access, invalid block number, already released block)
	*/
	err::FileError SimpleBlockOwner::release(int block_num) {
		//Verify block is not bad
		if (!verifyGoodAllocation(block_num))
			return err::CORRUPTED_BLOCK;
		//Release block
		singleRelease(block_num);
		return err::SUCCESS;
	}
	/* Release a set of blocks.
	block_num	>>	The identities of the blocks to release.
	return		>>	Success if no error occured on releasing the blocks (access, invalid block number, already released block)
	*/
	err::FileError SimpleBlockOwner::release(const std::vector<int>& block_num) {
		//Verify all blocks are OK
		for (int i = 0; i < block_num.size(); i++) {
			if (!verifyGoodAllocation(block_num[i]))
				return err::CORRUPTED_BLOCK;
		}
		//Release all blocks
		for (int i = 0; i < block_num.size(); i++)
			singleRelease(block_num[i]);
		return err::SUCCESS;
	}
	/* Get occupied blocks
	*/
	std::vector<int> SimpleBlockOwner::getOccupied() {
		std::vector<int> list(_status.size() - _free.size());
		int list_ind = 0;
		for (size_t i = 0; i < _status.size(); i++) {
			//If index is occupised add it to the list
			if (_status[i])
				list[list_ind++] = i;
		}
		return list;
	}
	/* Occupy a specified set of blocks. Formats owner if not empty.
	*/
	void SimpleBlockOwner::occupy(std::vector<int>& occupied) {
		if (_free.size() < _status.size())
			format();
		//Sort occupied list. Allows occupying in order and no rearrangment in the _free array.
		std::sort(occupied.begin(), occupied.end());
		//Skip weird blocks:
		if (occupied.back() >= _status.size())
			throw new std::out_of_range("Blocks called for occupation is out of range");
		//Occupy in reverse order due to sorted arrangment.
		for(size_t i = occupied.size() - 1; i >= 0; i--) {
			_free.erase(_free.begin() + occupied[i]);
			_status[occupied[i]] = true;
		}
	}

	/* Format all blocks releasing the data.
	*/
	void SimpleBlockOwner::format() {
		size_t size = _status.size();
		_status = std::vector<bool>(size, false);
		_free = std::vector<int>(size);
		//Add all blocks to the free list.
		for (size_t i = 0; i < _free.size(); i++)
			_free[i] = i;
	}

	/*	Get the number of free memory blocks
	*/
	size_t SimpleBlockOwner::free() {
		return _free.size();
	}
}
