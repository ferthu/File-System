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
	void addToDirectory(std::vector<std::string>& directory, std::stringstream& toAdd, const char& delim, const unsigned int& dirMaxSize, const FileSystem& fs);

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
	void directoryFromString(const std::string& str, const FileSystem& fs);

	// adds folder named in string to directory
	void addDirectory(const std::string str);

	// removes one folder from directory
	void removeDirectory();

	// << operator overload
	friend std::ostream& operator<<(std::ostream& os, const DirectoryReference& dr);

	// removes next character from stringstream if it equals argument
	bool removeCharacter(std::stringstream& stream, char c);
};

#endif
