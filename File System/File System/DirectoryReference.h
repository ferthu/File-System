#ifndef DIRECTORYREFERENCE_H
#define DIRECTORYREFERENCE_H

#include <string>
#include <vector>
#include <iostream>
#include "filesystem.h"

class DirectoryReference
{
private:
	void reportStreamError(const std::stringstream& stream);

	std::vector<std::string> directory;
	const unsigned int directoryMaxLength = 100;
	const char delimChar = '/';
	const char specialReferenceChar = '.';
public:
	DirectoryReference();
	~DirectoryReference();

	// returns stored directory
	const std::vector<std::string>& getDirectory() const;

	// creates and stores a directory from a string (relative or absolute)
	void directoryFromString(const DirectoryReference& currentDirectory, const std::string& str, const FileSystem& fs);

	// creates and stores a directory from a string (relative or absolute), treats last entry as a file and returns it
	void directoryAndFileFromString(const DirectoryReference& currentDirectory, const std::string& str, const FileSystem& fs, std::string& file);

	// adds folder named in string to directory
	void addDirectory(const std::string str, const FileSystem& fs);

	// removes one folder from directory
	void removeDirectory();

	// << operator overload
	friend std::ostream& operator<<(std::ostream& os, const DirectoryReference& dr);

	// removes next character from stringstream if it equals argument
	bool removeCharacter(std::stringstream& stream, char c);

	std::string getDirectoryString();

	// resets directory
	void format();
};

#endif
