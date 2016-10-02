#include "DirectoryReference.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

const std::vector<std::string>& DirectoryReference::getDirectory()
{
	return directory;
}

void DirectoryReference::directoryFromString(const std::string str)
{
	std::stringstream input(str);

	std::vector<std::string> newDirectory;

	if (input.good())
	{
		if (input.peek() == delimChar)
		{
			// absolute path
			input.get();

			addToDirectory(newDirectory, input, delimChar, directoryMaxLength);
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