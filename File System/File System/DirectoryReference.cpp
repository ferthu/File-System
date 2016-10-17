#include "DirectoryReference.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

const std::vector<std::string>& DirectoryReference::getDirectory() const
{
	return directory;
}

void DirectoryReference::directoryFromString(const std::string str)
{
	std::stringstream input(str);

	std::vector<std::string> newDirectory;

	if (input.good())
	{
		// absolute path
		if (input.peek() == delimChar)
		{
			addToDirectory(newDirectory, input, delimChar, directoryMaxLength);
		}
		// reference to current or parent directory
		else if (input.peek() == specialReferenceChar)
		{
			// remove first dot
			input.get();

			// two dots reference parent directory
			if (input.peek() == specialReferenceChar)
			{
				// remove dot
				input.get();

				newDirectory = directory;

				// remove lowest directory to obtain parent
				newDirectory.pop_back();

				addToDirectory(newDirectory, input, delimChar, directoryMaxLength);
			}
			// one dot references current directory
			else
			{
				newDirectory = directory;

				addToDirectory(newDirectory, input, delimChar, directoryMaxLength);
			}
		}
		else
		{
			// relative path
			newDirectory = directory;

			addToDirectory(newDirectory, input, delimChar, directoryMaxLength);
		}
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

void DirectoryReference::addToDirectory(std::vector<std::string>& directory, std::stringstream& toAdd, const char& delim, const unsigned int& dirMaxSize)
{
	char* dir = new char[dirMaxSize];

	while (toAdd.good())
	{
		// remove delim character
		if(toAdd.peek() == delim)
			toAdd.get();

		toAdd.get(dir, dirMaxSize, delim);

		// TODO: check if directory exists

		directory.push_back(std::string(dir));
	}

	delete[] dir;

	if (!toAdd.eof())
		throw std::invalid_argument("Error: string error\n");
}

void DirectoryReference::addDirectory(const std::string str)
{
	// TODO: check if directory exists

	directory.push_back(str);
}

void DirectoryReference::removeDirectory()
{
	directory.pop_back();
}

DirectoryReference::DirectoryReference() {}
DirectoryReference::~DirectoryReference() {}

std::ostream& operator<<(std::ostream& os, const DirectoryReference& dr)
{
	int size = dr.directory.size();

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