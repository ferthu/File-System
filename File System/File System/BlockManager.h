#pragma once

#include"memblockdevice.h"

namespace file {

	/* Block manager takes care of the entire management of the data stored on the virtual disk.
	*/
	class BlockManager
	{
	private:
		/* The virtual disk
		*/
		MemBlockDevice _disk;

	public:


		BlockManager(unsigned int num_blocks = 512);
		virtual ~BlockManager();
	};

}
