#pragma once

#include"BinaryWriter.h"

namespace mf {

	class BinaryFileWriter 
		: public BinaryWriter
	{
	private:
		/* File output stream
		*/
		std::ofstream _file_stream;
	public:
		BinaryFileWriter();
		virtual ~BinaryFileWriter();
		/* Try to open a file for writing
		directory	<<	File directory & name
		return		>>	True if the file is open
		*/
		bool openFile(const std::string &directory);
		/* Try to open a file for writing
		directory	<<	File directory & name
		return		>>	True if the file is open
		*/
		bool openFile(const std::wstring &directory);
		/* Verifies if the stream is open or not.
		*/
		bool isOpen();
		/* Close the stream
		*/
		void close();
	};

}

