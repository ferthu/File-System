#ifndef MEMBLOCKDEVICE_H
#define MEMBLOCKDEVICE_H
#include "blockdevice.h"

class MemBlockDevice: public BlockDevice
{
public:
    MemBlockDevice(unsigned int nrOfBlocks = 250);
    MemBlockDevice(const MemBlockDevice &other);

	virtual ~MemBlockDevice();

    /* Operators */
    MemBlockDevice& operator=(const MemBlockDevice &other);
    Block &operator[] (int index) const;

    /* Returns amount of free blocks */
    int spaceLeft() const;

    /* Writes a block */
    int writeBlock(int blockNr, const std::vector<char> &vec);
    int writeBlock(int blockNr, const std::string &strBlock);
    int writeBlock(int blockNr, const char cArr[]);     // Use with care, make sure that cArr has at least the same dimension as block


    /* Reads a block */
    Block readBlock(int blockNr) const;
	/* Read a set of blocks.
	*/
	std::string readBlock(const std::vector<int> blocks, size_t bytes) const;

    /* Resets all the blocks */
    void reset();

    /* Return the size */
    int size() const;
};

#endif // MEMBLOCKDEVICE_H
