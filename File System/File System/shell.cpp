#include <iostream>
#include <sstream>
#include "filesystem.h"
#include "FileError.h"
#include "DirectoryReference.h"
#include"FileSystemHandle.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 16;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help","chmod"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
std::string help();

int main(void) {

	std::string userCommand, tmp, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";   // Change this if you want another user to be displayed
	file::FileSystemHandle _handle;
    bool bRun = true;

    do {
		try
		{
			std::cout << user << ":" << _handle << "$ ";
			getline(std::cin, userCommand);

			int nrOfCommands = parseCommandString(userCommand, commandArr);
			if (nrOfCommands > 0) {

				int cIndex = findCommand(commandArr[0]);
				switch (cIndex) {

				case 0: // quit
					bRun = false;
					std::cout << "Exiting\n";
					break;
				case 1: // format
					_handle.format();
					break;
				case 2: // ls
					std::cout << _handle.listDirectory();
					break;
				case 3: // create
					std::cout << "Write file data:\n";
					getline(std::cin, tmp);
					_handle.createFile(commandArr[1], 4, tmp);
					break;
				case 4: // cat
					std::cout << _handle.printFile(commandArr[1]);
					break;
				case 5: // createImage
					_handle.createImage(commandArr[1]);
					break;
				case 6: // restoreImage
					_handle.readImage(commandArr[1]);
					break;
				case 7: // rm
					_handle.remove(commandArr[1]);
					break;
				case 8: // cp
					_handle.copyFile(commandArr[1], commandArr[2]);
					break;
				case 9: // append
					_handle.appendFile(commandArr[1], commandArr[2]);
					break;
				case 10: // mv
					_handle.move(commandArr[1], commandArr[2]);
					break;
				case 11: // mkdir
					_handle.createFolder(commandArr[1]);
					break;
				case 12: // cd
					_handle.cd(commandArr[1]);
					break;
				case 13: // pwd
					std::cout << _handle.getWorkingPath() << std::endl;
					break;
				case 14: // help
					std::cout << help() << std::endl;
					break;
				case 15: // chmod
					_handle.setRights(commandArr[2], commandArr[1]);
					break;
				default:
					std::cout << "Unknown command: " << commandArr[0] << std::endl;
				}
			}
		}
		catch (err::FileError e)
		{
			std::cout << "File error: " << err::getMsg(e) << std::endl;
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << e.what();
		}
		catch (std::out_of_range e)
		{
			std::cout << e.what();
		}
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:								Quit OSD Disk Tool\n";
    helpStr += "* format:							Formats disk\n";
    helpStr += "* ls:								Lists contents of current directory.\n";
    helpStr += "* create:							Creates a file and stores contents in current directory.\n";
    helpStr += "* cat <file>:							Dumps contents of <file>.\n";
    helpStr += "* createImage <real-file>:					Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:					Reads <real-file> onto disk\n";
    helpStr += "* rm <file>:							Removes <file>\n";
    helpStr += "* cp <src file dir> <dst file dir>:				Copy <src file> to <dst file>\n";
    helpStr += "* append <src file dir> <dst file dir>:				Appends contents of <src file> to <dst file>\n";
    helpStr += "* mv <src file dir> <dst file dir>:				Renames or moves <src file> to <dst file>\n";
    helpStr += "* mkdir <directory>:						Creates a new directory called <directory>\n";
    helpStr += "* cd <directory>:						Changes current working directory to <directory>\n";
    helpStr += "* pwd:								Get current working directory\n";
	helpStr += "* chmod <rights> <file>:					Edits access rights of <file>. 0: No access, 1: Write Only, 2: Read Only, >2: Read & Write\n";
    helpStr += "* help:								Prints this help screen\n";
    return helpStr;
}

