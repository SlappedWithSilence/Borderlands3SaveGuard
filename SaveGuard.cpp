/* Software by Aaron Sigal
 * Do not redistribute.
 * I make no warranty or guarentee of any kind.
 * Feel free to modify, but include me as a credit and provide a link to my Github
*/

// C++ Libs
#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;

//Forward declerations

int BackupSaves();
bool LaunchBorderlands();
bool CopyFile(fs::path src, fs::path dest);

// Const values

const std::string SHORTCUT_FILE_NAME = "Borderlands3.lnk";
const std::string PROFILE_FILE_NAME = "profile.sav";
const std::string SAVE_EXTENSION = ".sav";
const std::string BORDERLANDS_LAUNCH_FAILED = "Launching Borderlands failed, is the correct shortcut present in this folder?\n";
const int TIME_TO_BACKUP = 10 * 60; // Amount of time the program will wait before calling BackupSaves().
const int MAX_NUMBER_OF_SAVES = 5;

int main() {
	// Intro Prompt

	std::cout << "Please note, this program will not run correctly if it is not inside your Borderlands 3 save folder." << std::endl;
	std::cout << "If it isn't, please close this window and move it now." << std::endl;

	std:: cout << "Total number of characters backed up: " << BackupSaves() << std::endl;
	
	if (!LaunchBorderlands()) {
		std::cout << BORDERLANDS_LAUNCH_FAILED;
		return -1;
	}

	auto last_backup = std::time(0);;

	while(true) {
		if (std::time(0) - last_backup >= TIME_TO_BACKUP) {
			last_backup = std::time(0);;
			
			std::cout << "Backing up...";
			BackupSaves();
			std:: cout << " done!" << std::endl;
		} else {
			std::cout << ".";
		}
	}
}

int BackupSaves() {
	int successes = 0;

	for (int i = 0; i < MAX_NUMBER_OF_SAVES; ++i) {
		std::string file_name = std::to_string(i) + SAVE_EXTENSION;

		std::ifstream ifs;
		ifs.open(file_name);

		if(ifs.good()) {
			ifs.close();

			if (CopyFile(file_name, ("backup/" + file_name) )) successes++; 

		} else {
			ifs.close();
		}
	}

	CopyFile(PROFILE_FILE_NAME, ("backup/" + PROFILE_FILE_NAME) ); // Backup the profile save file

	return successes;
}

bool CopyFile(fs::path src, fs::path dest) {
	fs::path sourceFile = src;
    fs::path targetParent = dest;
    auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".

    try 
    {
        fs::create_directories(targetParent); // Recursively create target directory if not existing.
        fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
    }
    catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
    {
        return false;
    }

    return true;
}

bool LaunchBorderlands() {
	std::ifstream ifs;
		ifs.open(SHORTCUT_FILE_NAME);

		if(ifs.good()) { // Check if the shortcut exists
			ifs.close();
			 ShellExecute(NULL, "open", SHORTCUT_FILE_NAME.c_str(), NULL, NULL, SW_SHOWDEFAULT); // Launch Borderlands through the shortcut using a Shell window.
		} else {
			ifs.close();
			return false;
		}


	return true;
}