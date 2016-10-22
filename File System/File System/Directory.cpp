#include "Directory.h"

namespace file {

	Directory::Directory(const std::string& name)
		: _name(name), _directories(), _files(), _byte_size(0), _parent(nullptr)
	{
	}


	Directory::~Directory()
	{
	}

	/* Compares a directory name
	*/
	bool Directory::compareDirectoryName(const std::string& dir_name) {
		return dir_name == _name;
	}
	int Directory::getDirectory(const std::string& dir_name) {
		for (size_t i = 0; i < _directories.size(); i++) {
			if (_directories[i]->compareDirectoryName(dir_name))
				return (int)i;
		}
		return -1;
	}

	/* Update a change in byte size
	difference	<<	Byte size the directory is changed with
	*/
	void Directory::updateSize(int difference) {
		_byte_size += difference;
		if(_parent)
			_parent->updateSize(difference);
	}

	/* Removes a specific directory from index.
	i			<<	Index to remove
	byte_size	<<	The byte size of the directory
	*/
	void Directory::removeDirectory(int i, int byte_size) {
		updateSize(-byte_size);
		_directories.erase(_directories.begin() + i);
	}

	/* Access a directory. Returns the file node reference of the accessed directory or directory node an error occured.
	*/
	DirectoryAccess Directory::accessDirectory(const std::vector<std::string>& directory, unsigned int traversal_lvl) {
		/* Check if destination directory is reached
		*/
		if (traversal_lvl == directory.size())
			return DirectoryAccess(traversal_lvl, this, err::SUCCESS);
		/* Traverse the tree
		*/
		int dir_index = getDirectory(directory[traversal_lvl]);
		if (dir_index >= 0)
			return _directories[dir_index]->accessDirectory(directory, ++traversal_lvl);
		/*	No child directory with specified name exists
		*/
		return DirectoryAccess(traversal_lvl + 1, nullptr, err::DIRECTORY_DOES_NOT_EXIST);
	}
	/* Add a child directory to the node
	*/
	void Directory::addDirectory(std::unique_ptr<Directory>& child) {
		child->_parent = this;
		_directories.push_back(std::move(child));
	}
	/* Remove a child directory in the node
	name	<<	Name of the directory to remove
	return	>>	If the directory exists
	*/
	err::FileError Directory::removeDirectory(const std::string& name) {
		int dir_index = getDirectory(name);
		if (dir_index >= 0) {
			removeDirectory(dir_index, _directories[dir_index]->_byte_size);
			return err::SUCCESS;
		}
		return err::FOLDER_DOES_NOT_EXIST;
	}
	/* Move ownership of a child directory or file
	*/
	err::FileError Directory::moveChild(const std::string& name, const std::string& new_name, IDirectoryReference& move_to) {
		int dir_index = getDirectory(name);
		if (dir_index >= 0) {
			_directories[dir_index]->_name = new_name;
			int byte_size = _directories[dir_index]->_byte_size;
			move_to.addDirectory(_directories[dir_index]);
			removeDirectory(dir_index, byte_size);
			return err::SUCCESS;
		}
		FileReference ref;
		if (err::good(getFile(name, ref))) {
			move_to.addFile(FileReference(new_name, ref._block, ref._byte_size));
			removeFile(ref);
			return err::SUCCESS;
		}
		return err::NOT_FOUND;
	}
	/* Get a file
	name		<<	Name of the file
	reference	>>	Return reference of the found file
	return		>>	Returns if file was successfully found or if an error occured.
	*/
	err::FileError Directory::getFile(const std::string& name, FileReference& reference) {
		for (size_t i = 0; i < _files.size(); i++) {
			if (_files[i] == name) {
				reference = _files[i];
				return err::SUCCESS;
			}
		}
		return err::FILE_NOT_FOUND;
	}
	/* Add a file reference. If a reference with same name exists old reference is deleted.
	ref		<<	Reference to the file
	*/
	void Directory::addFile(const FileReference& ref) {
		FileReference tmp;
		//Remove any existing file
		removeFile(ref);
		//Add new reference
		_files.push_back(ref);
		updateSize(ref._byte_size);
	}
	/* Remove a file
	ref		<<	Reference to the file
	return	>>	Success if no error occured.
	*/
	err::FileError Directory::removeFile(const FileReference& ref) {
		for (size_t i = 0; i < _files.size(); i++) {
			if (_files[i] == ref._name) {
				updateSize(-(int)_files[i]._byte_size);
				_files.erase(_files.begin() + i);
				return err::SUCCESS;
			}
		}
		return err::FILE_NOT_FOUND;
	}
	/* Get the file names
	*/
	std::vector<std::string> Directory::getFileNames() {
		std::vector<std::string> names(_files.size());
		for (size_t i = 0; i < _files.size(); i++)
			names[i] = _files[i]._name + "\t" + std::to_string(_files[i]._byte_size);
		return names;
	}
	/* Get the names of the child directories
	*/
	std::vector<std::string> Directory::getDirectoryNames() {
		std::vector<std::string> names(_directories.size());
		for (size_t i = 0; i < _directories.size(); i++)
		{
			names[i] = _directories[i]->_name + "\t" + std::to_string(_directories[i]->_byte_size);
		}
		return names;
	}
	/* Get if the name is a directory or a file type
	*/
	type::Dir Directory::getType(const std::string& name) {
		FileReference ref;
		if (err::good(getFile(name, ref)))
			return type::File;
		if (getDirectory(name) >= 0)
			return type::Folder;
		return type::None;
	}


	/* Writer to stream
	*/
	void Directory::writeToStream(mf::BinaryFileWriter& writer) {
		writer.writeString(_name);

		//Write file references
		writer.writeUInt((unsigned int)_files.size());
		for (int i = 0; i < _files.size(); i++) {
			writer.writeString(_files[i]._name);
			writer.writeInt(_files[i]._block);
			writer.writeUInt(_files[i]._byte_size);
		}
		//Write directories
		writer.writeUInt((unsigned int)_directories.size());
		for (int i = 0; i < _directories.size(); i++)
			_directories[i]->writeToStream(writer);
	}
	std::unique_ptr<Directory> Directory::readFromStream(mf::BinaryFileReader& reader) {

		std::string name = reader.readString();
		std::unique_ptr<Directory> dir(new Directory(name));
		//Read files
		unsigned int fileNum = reader.readUInt();
		for (unsigned int i = 0; i < fileNum; i++) {
			//Read in order. Create reference first
			FileReference ref;
			ref._name = reader.readString();
			ref._block = reader.readInt();
			ref._byte_size = reader.readUInt();
			dir->addFile(ref);
		}
		//Read child directories
		unsigned int dirNum = reader.readUInt();
		for (unsigned int i = 0; i < dirNum; i++)
			dir->addDirectory(readFromStream(reader));
		return std::move(dir);
	}
}