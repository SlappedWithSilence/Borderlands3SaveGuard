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
#include <stdio.h>
#include <io.h>

namespace fs = std::filesystem;

//Forward declerations

int BackupSaves();
bool LaunchBorderlands();
bool CopyFile(std::string source, std::string dest);

// Const values

const std::string BACKUP_FOLDER_PREFIX = "backups/";
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

	// Startup backup
	mkdir("backups");
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
			std:: cout << "Total number of characters backed up: " << BackupSaves() << std::endl;
			std:: cout << " done!" << std::endl;
		} else {
			//std::cout << ".";
		}
	}
}

int BackupSaves() {
	int successes = 0;

	for (int i = 0; i < MAX_NUMBER_OF_SAVES; ++i) {
		std::string file_name = std::to_string(i + 1) + SAVE_EXTENSION;

		std::ifstream ifs;
		ifs.open(file_name);

		if(ifs.good()) {
			ifs.close();

			if (CopyFile(file_name, ("backups/" + file_name) )) successes++; 

		} else {
			ifs.close();
			std::cout << "Couldn't locate " << file_name << "!\n";
		}
	}

	CopyFile(PROFILE_FILE_NAME, ("backups/" + PROFILE_FILE_NAME) ); // Backup the profile save file

	return successes;
}

bool CopyFile(std::string source, std::string dest) {
	std::ifstream  src(source,    std::ios::binary);

	if (src.good()) {
 		std::ofstream  dst(dest,   std::ios::binary);
 		dst << src.rdbuf();
 		dst.close();
 		src.close();
	} else {
		std::cout << "Failed to read " << source << "\n";
		src.close();
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