#pragma once

#include"Directory.h"
#include"File.h"
#include<memory>
#include"BinaryFileWriter.h"
#include"BinaryFileReader.h"

namespace file {
	
	
	/*	Wrapper containing the root node and access functions to the file tree.
	*/
	class DirectoryTree
	{
	private:

		/* Root node.
		*/
		std::unique_ptr<Directory> _root;
		explicit DirectoryTree(const DirectoryTree& root) = delete;

	public:
		/* Create a file tree from the root reference.
		*/
		explicit DirectoryTree(std::unique_ptr<Directory>& root);
		DirectoryTree(DirectoryTree&& root);

		DirectoryTree& operator=(DirectoryTree&& move);

		~DirectoryTree();

		/* Get a node in the file tree.
		*/
		DirectoryAccess accessDirectory(const std::vector<std::string>& directory);
		/* Get a node in the file tree.
		*/
		const DirectoryAccess accessDirectory(const std::vector<std::string>& directory) const;
		/* Write to stream
		*/
		void writeToStream(mf::BinaryFileWriter& writer);
		/* Read from stream.
		*/
		static DirectoryTree readFromStream(mf::BinaryFileReader& reader);

		/* Format directory tree
		*/
		void format();
	};

}

