#pragma once

#include<streambuf>
#include<vector>
#include<queue>
#include<memory>
#include"blockdevice.h"
#include"FileError.h"

namespace file {

	class VirtualStream
		: public std::streambuf
	{
	private:

		err::FileError _err;

		/* The block device reference
		*/
		BlockDevice& _device;

		/* Queue managing next reading block.
		*/
		std::queue<int> _block_queue;

		/* Current buffer
		*/
		Block _buffer;
		/* Copy constructor not allowed	*/
		VirtualStream(const VirtualStream &) = delete;
		/* Copy operator not allowed */
		VirtualStream &operator= (const VirtualStream &) = delete;

		/* Set put pointers to current buffer. Offsets the pptr with the offset.
		*/
		void setp_cur(size_t offset);
		/* Set get pointers to current buffer. Offsets the gptr with the offset.
		*/
		void setg_cur(size_t offset);

		/* Put a character into the stream */
		virtual int overflow(int c);
		/* No reason to overwrite underflow as it works  stream */
		virtual int underflow();

		/* Buffer a block for reading
		*/
		bool bufferNext();

		/* Queue a block for the stream */
		void queue_block(int block);
		/* Queue blocks for accessing stream
		blocks	<<	Blocks to queue
		*/
		void queue_block(const std::vector<int>& blocks);
	public:
		/* Open a file block for reading
		*/
		err::FileError openRead(int block);
		/* Open a file block for reading
		*/
		err::FileError openRead(const std::vector<int>& block);
		/* Open a designated file blocks for writing
		*/
		err::FileError openWrite(int block);
		/* Open a designated file blocks for writing
		*/
		err::FileError openWrite(const std::vector<int>& block);
		/* Create an small write stream
		*/
		VirtualStream(BlockDevice& device);
		virtual ~VirtualStream();
		/* Flush write stream
		*/
		err::FileError close();
		/* Get the current error
		*/
		err::FileError getErr();
		/* Get the size remaining of the buffered block
		*/
		unsigned int getBlockSize();
	};
}

