#include "DirectoryReference.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

const std::vector<std::string>& DirectoryReference::getDirectory() const
{
	return directory;
}

void DirectoryReference::directoryFromString(const DirectoryReference& currentDirectory, const std::string& str, const FileSystem& fs)
{
	std::stringstream input(str);

	std::vector<std::string> newDirectory;

	if (input.good())
	{
		// absolute path
		if (input.peek() == delimChar)
		{
			addToDirectory(newDirectory, input, delimChar, directoryMaxLength, fs);
		}
		// reference to current or parent directory
		else if (input.peek() == specialReferenceChar)
		{
			// remove first dot
			removeCharacter(input, specialReferenceChar);

			// two dots reference parent directory
			if (input.peek() == specialReferenceChar)
			{
				// remove dot
				removeCharacter(input, specialReferenceChar);

				newDirectory = currentDirectory.getDirectory();

				// remove lowest directory to obtain parent
				if (newDirectory.size() > 0)
					newDirectory.pop_back();

				addToDirectory(newDirectory, input, delimChar, directoryMaxLength, fs);

			}
			// one dot references current directory
			else
			{
				removeCharacter(input, delimChar);

				newDirectory = currentDirectory.getDirectory();

				addToDirectory(newDirectory, input, delimChar, directoryMaxLength, fs);
			}
		}
		else
		{
			// relative path
			newDirectory = currentDirectory.getDirectory();

			addToDirectory(newDirectory, input, delimChar, directoryMaxLength, fs);
		}

		directory = newDirectory;
	}
	else
	{
		reportStreamError(input);
	}
}

std::string DirectoryReference::directoryAndFileFromString(const DirectoryReference& currentDirectory, const std::string& str, const FileSystem& fs)
{
	std::stringstream input(str);

	std::vector<std::string> newDirectory;

	std::string file;

	if (input.good())
	{
		// absolute path
		if (input.peek() == delimChar)
		{
			file = addToDirectoryWithFile(newDirectory, input, delimChar, directoryMaxLength, fs);
		}
		// reference to current or parent directory
		else if (input.peek() == specialReferenceChar)
		{
			// remove first dot
			removeCharacter(input, specialReferenceChar);

			// two dots reference parent directory
			if (input.peek() == specialReferenceChar)
			{
				// remove dot
				removeCharacter(input, specialReferenceChar);

				newDirectory = currentDirectory.getDirectory();

				// remove lowest directory to obtain parent
				if (newDirectory.size() > 0)
					newDirectory.pop_back();

				file = addToDirectoryWithFile(newDirectory, input, delimChar, directoryMaxLength, fs);

			}
			// one dot references current directory
			else
			{
				removeCharacter(input, delimChar);

				newDirectory = currentDirectory.getDirectory();

				file = addToDirectoryWithFile(newDirectory, input, delimChar, directoryMaxLength, fs);
			}
		}
		else
		{
			// relative path
			newDirectory = currentDirectory.getDirectory();

			file = addToDirectoryWithFile(newDirectory, input, delimChar, directoryMaxLength, fs);
		}

		directory = newDirectory;

		return file;
	}
	else
	{
		reportStreamError(input);
		return "Error"; //Code does not reach this point (error is thrown).
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

void DirectoryReference::addToDirectory(std::vector<std::string>& directory, std::stringstream& toAdd, const char& delim, const unsigned int& dirMaxSize, const FileSystem& fs)
{
	if (toAdd.peek() == -1)
		return;

	if (toAdd.good())
	{
		char* dir = new char[dirMaxSize];

		while (toAdd.good())
		{
			// remove delim character
			removeCharacter(toAdd, delimChar);

			if (toAdd.good())
			{
				toAdd.get(dir, dirMaxSize, delim);

				// check if directory exists
				directory.push_back(std::string(dir));
				err::FileError err;
				if (err::bad(err = fs.directoryExists(directory)))
				{
					throw err;
				}					
			}
		}

		delete[] dir;

		if (!toAdd.eof())
			throw std::invalid_argument("Error: string error\n");
	}
	else
		throw std::invalid_argument("Error: addToDirectory received bad input\n");
}

std::string DirectoryReference::addToDirectoryWithFile(std::vector<std::string>& directory, std::stringstream& toAdd, const char& delim, const unsigned int& dirMaxSize, const FileSystem& fs)
{
	if (toAdd.peek() == -1)
		throw std::invalid_argument("Error: bad directory\n");

	std::string file;

	if (toAdd.good())
	{
		char* dir = new char[dirMaxSize];

		while (toAdd.good())
		{
			// remove delim character
			removeCharacter(toAdd, delimChar);

			if (toAdd.good())
			{
				toAdd.get(dir, dirMaxSize, delim);

				if (toAdd.eof())
				{
					// this entry refers to a file
					file = dir;
				}
				else
				{
					// check if directory exists
					std::vector<std::string> dirs;

					err::FileError err;
					if (err::good(err = fs.directoryExists(directory)))
					{
						directory.push_back(std::string(dir));
					}
					else
						throw err;
				}
			}
		}

		delete[] dir;

		if (!toAdd.eof())
			throw std::invalid_argument("Error: string error\n");

		return file;
	}
	else
		throw std::invalid_argument("Error: addToDirectoryWithFile received bad input\n");
}

void DirectoryReference::addDirectory(const std::string str)
{
	// TODO: check if directory exists

	directory.push_back(str);
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