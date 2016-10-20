#include "VirtualStream.h"

namespace file {

	VirtualStream::VirtualStream(BlockDevice& device)
		: _device(device), _buffer(), _block_queue(), _err(err::STREAM_CLOSED)
	{
		//Define this as no-buffer
		setg(nullptr, nullptr, nullptr);
		setp(nullptr, nullptr, nullptr);
	}


	VirtualStream::~VirtualStream()
	{
	}
	/* Get the current error
	*/
	err::FileError VirtualStream::getErr() {
		return _err;
	}
	/* Open a file block for reading
	*/
	err::FileError VirtualStream::openRead(int block) {
		queue_block(block);
		if (bufferNext())
			_err = err::SUCCESS;
		return _err;
	}
	/* Open a file block for reading
	*/
	err::FileError VirtualStream::openRead(const std::vector<int>& block) {
		queue_block(block);
		if (bufferNext())
			_err = err::SUCCESS;
		return _err;
	}
	/* Open a file block for writing
	*/
	err::FileError VirtualStream::openWrite(int block) {
		queue_block(block);
		//Set write pointers to the buffer.
		setp_cur(0);
		_err = err::SUCCESS;
		return _err;
	}
	/* Open a designated file blocks for writing
	*/
	err::FileError VirtualStream::openWrite(const std::vector<int>& block) {
		queue_block(block);
		//Set write pointers to the buffer.
		setp_cur(0);
		_err = err::SUCCESS;
		return _err;
	}

	/* Set put pointers to current buffer. Offsets the pptr with the offset.
	*/
	void VirtualStream::setp_cur(size_t offset) {
		char* begin = _buffer.begin();
		char* end = _buffer.end();
		/* Set the write (put) partition available */
		setp(begin, begin + offset, end);
	}
	/* Set get pointers to current buffer. Offsets the gptr with the offset.
	*/
	void VirtualStream::setg_cur(size_t offset) {
		char* begin = _buffer.begin();
		char* end = _buffer.end();
		/* Set the partition of read information available in the container */
		setg(begin, begin + offset, end);
	}
	/* Queue a block for the stream */
	void VirtualStream::queue_block(int block) {
		_block_queue.push(block);
	}
	/* Queue blocks for accessing stream
	blocks	<<	Blocks to queue
	*/
	void VirtualStream::queue_block(const std::vector<int>& blocks) {
		for (unsigned int i = 0; i < blocks.size(); i++)
			_block_queue.push(blocks[i]);
	}

	/* Put a character into next buffer */
	int VirtualStream::overflow(int c) {
		//Verify this is a write buffer
		if (pptr() == nullptr) {
			_err = err::INVALID_WRITE_BUFFER;
			return traits_type::eof();
		}
		//Verify queue
		if (_block_queue.empty()) {
			_err = err::WRITE_BUFFER_OVERFLOW;
			return traits_type::eof();
		}
		//Write the buffer.
		if (0 > _device.writeBlock(_block_queue.front(), _buffer.begin())) {
			_err = err::INVALID_BLOCK;
			return traits_type::eof();
		}
		_block_queue.pop();
		//Reset write pointers
		setp_cur(0);
		//Verify there is another block to write
		if (_block_queue.empty())
			return traits_type::eof();
		
		//Set c
		_buffer.begin()[0] = traits_type::to_char_type(c);
		return c;
	}

	int VirtualStream::underflow() {
		//Verify this is a read buffer
		if (gptr() == nullptr) {
			_err = err::INVALID_READ_BUFFER;
			return traits_type::eof();
		}
		//Open next read buffer in queue and pop it.
		if (!bufferNext())
			return traits_type::eof();
		_block_queue.pop();
		return *gptr();
	}
	/* Buffer a block for reading
	*/
	bool VirtualStream::bufferNext() {
		//Verify queue
		if (_block_queue.empty()) {
			_err = err::READ_BUFFER_OVERFLOW;
			return false;
		}
		//Try to get next buffer
		try {
			_buffer = _device.readBlock(_block_queue.front());
			_block_queue.pop();
			setg_cur(0);
		}
		catch (std::out_of_range& e) {
			_err = err::INVALID_BLOCK;
			return false;
		}
		return true;
	}
	/* Flush write stream
	*/
	err::FileError VirtualStream::close() {
		//Verify that this is a write buffer with remaining data and everything is OK
		if (pptr() == nullptr || _block_queue.empty() || err::bad(_err))
			return _err;
		//Flush the write buffer to virtual disk.
		if (0 > _device.writeBlock(_block_queue.front(), _buffer.begin()))
			_err = err::INVALID_BLOCK;
		setp(nullptr, nullptr, nullptr);
		setg(nullptr, nullptr, nullptr);
		//Flush queue
		_block_queue = std::queue<int>();
		return _err;
	}

}