#include "DirectoryReference.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

const std::vector<std::string>& DirectoryReference::getDirectory() const
{
	return directory;
}

void DirectoryReference::setDirectory(const std::vector<std::string>& directory)
{
	this->directory = directory;
}

void DirectoryReference::directoryFromString(const DirectoryReference& currentDirectory, const std::string& str, const FileSystem& fs)
{
	std::stringstream input(str);

	std::vector<std::string> newDirectory;

	if (input.good())
	{
		// absolute path
		if (input.peek() == delimChar)
			removeCharacter(input, delimChar);

		// relative path, begin at current directory
		else
			newDirectory = currentDirectory.getDirectory();

		if (input.good())
		{
			const int dirMaxSize = 100;
			char* dir = new char[dirMaxSize];

			while (input.good())
			{
				// remove delim character
				removeCharacter(input, delimChar);

				if (input.good())
				{
					input.get(dir, dirMaxSize, delimChar);
					if (dir[0] == specialReferenceChar)
					{
						if (dir[1] == specialReferenceChar)
						{
							// references parent directory
							if (!newDirectory.empty())
								newDirectory.pop_back();
						}
						else if (dir[1] == delimChar)
						{
							// references this directory, do nothing
						}
					}
					else
					{
						// check if directory exists
						newDirectory.push_back(std::string(dir));
						err::FileError err;
						if (err::bad(err = fs.directoryExists(newDirectory)))
						{
							newDirectory.pop_back();
							throw err;
						}
					}
				}
			}

			delete[] dir;

			if (!input.eof())
				throw std::invalid_argument("Error: string error\n");
		}
		// set directory of this instance to the found directory
		directory = newDirectory;
	}
	else
	{
		reportStreamError(input);
	}
}

void DirectoryReference::directoryAndFileFromString(const DirectoryReference& currentDirectory, const std::string& str, const FileSystem& fs, std::string& file)
{
	std::stringstream input(str);

	std::vector<std::string> newDirectory;

	if (input.good())
	{
		// absolute path
		if (input.peek() == delimChar)
			removeCharacter(input, delimChar);

		// relative path, begin at current directory
		else
			newDirectory = currentDirectory.getDirectory();

		if (input.good())
		{
			const int dirMaxSize = 100;
			char* dir = new char[dirMaxSize];

			while (input.good())
			{
				// remove delim character
				removeCharacter(input, delimChar);

				if (input.good())
				{
					input.get(dir, dirMaxSize, delimChar);

					if (input.eof())
					{
						// this is the last entry, witch refers to a file
						file = dir;
					}
					else if (dir[0] == specialReferenceChar)
					{
						if (dir[1] == specialReferenceChar)
						{
							// references parent directory
							if (!newDirectory.empty())
								newDirectory.pop_back();
						}
						else if (dir[1] == delimChar)
						{
							// references this directory, do nothing
						}
					}
					else
					{
						// check if directory exists
						newDirectory.push_back(std::string(dir));
						err::FileError err;
						if (err::bad(err = fs.directoryExists(newDirectory)))
						{
							newDirectory.pop_back();
							throw err;
						}
					}
				}
			}

			delete[] dir;

			if (!input.eof())
				throw std::invalid_argument("Error: string error\n");
		}
		// set directory of this instance to the found directory
		directory = newDirectory;
	}
	else
	{
		reportStreamError(input);
	}
}

void DirectoryReference::reportStreamError(const std::stringstream& stream)
{
	// empty string
	if (stream.eof())
		throw std::invalid_argument("Error: empty input string\n");
	// other error
	else
		throw std::invalid_argument("Error: string error\n");
}

void DirectoryReference::addDirectory(const std::string str, const FileSystem& fs)
{
	directory.push_back(str);

	err::FileError err;
	if (err::bad(err = fs.directoryExists(this->getDirectory())))
	{
		directory.pop_back();
		throw(err);
	}
}

void DirectoryReference::removeDirectory()
{
	if (!directory.empty())
		directory.pop_back();
}

DirectoryReference::DirectoryReference() {}
DirectoryReference::~DirectoryReference() {}

std::ostream& operator<<(std::ostream& os, const DirectoryReference& dr)
{
	int size = (int)dr.directory.size();

	if (size < 1)
	{
		os << dr.delimChar;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			os << dr.delimChar << dr.directory[i];
		}
	}

	return os;
}

bool DirectoryReference::removeCharacter(std::stringstream& stream, char c)
{
	if (stream.peek() == c)
	{
		stream.get();

		if (stream.peek() == -1)
			stream.get();

		return true;
	}
	return false;
}

std::string DirectoryReference::getDirectoryString()
{
	int size = (int)directory.size();
	std::string output;

	if (size < 1)
	{
		output = delimChar;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			output += delimChar + directory[i];
		}
	}

	return output;
}

void DirectoryReference::format()
{
	directory.clear();
}