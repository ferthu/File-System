#include "BlockManager.h"

namespace file {



	BlockManager::BlockManager(unsigned int num_blocks)
		: _disk(num_blocks)
	{
	}


	BlockManager::~BlockManager()
	{
	}


}