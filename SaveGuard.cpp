/* Software by Aaron Sigal
 * Do not redistribute.
 * I make no warranty or guarentee of any kind.
 * Feel free to modify, but include me as a credit and provide a link to my Github
*/

// C++ Libs
#include <ctime>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <vector>

//Forward declerations

int BackupSaves();
bool LaunchBorderlands();

bool CopyFile(std::string source, std::string dest);

bool CreateConfigFile(std::string file_name);
bool WriteProperty(std::string file_name, std::string prop_name, std::string prop_value);

// Directory Constants
const std::string ROOT_FOLDER_PREFIX   = "SaveGuard/";
const std::string BACKUP_FOLDER_PREFIX = ROOT_FOLDER_PREFIX + "backups/";
const std::string CONFIG_FOLDER_PREFIX = ROOT_FOLDER_PREFIX + "config/";

// Filename Constants
const std::string SHORTCUT_FILE_NAME   = "Borderlands3.lnk";
const std::string PROFILE_FILE_NAME    = "profile.sav";
const std::string CONFIG_FILE_NAME     = "config.cfg";
const std::string SAVE_EXTENSION       = ".sav";

// Prompt Constants
const std::string BORDERLANDS_LAUNCH_FAILED = "Launching Borderlands failed, is the correct shortcut present in this folder?\n";

// Functionality Constants
const int TIME_TO_BACKUP      = 10 * 60; // Amount of time the program will wait before calling BackupSaves().
const int MAX_NUMBER_OF_SAVES = 5;
const int NUMBER_OF_BACKUPS   = 1;

int main() {
	// Intro Prompt

	std::cout << "Please note, this program will not run correctly if it is not inside your Borderlands 3 save folder." << std::endl;
	std::cout << "If it isn't, please close this window and move it now." << std::endl;

	// Startup backup
	mkdir(ROOT_FOLDER_PREFIX.c_str());
	mkdir(BACKUP_FOLDER_PREFIX.c_str());
	mkdir(CONFIG_FOLDER_PREFIX.c_str();

	
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
			std::cout << "\nCouldn't locate " << file_name << "!\n";
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
		std::cout << "\nFailed to read " << source << "\n";
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

// Generates an empty config file if one doesn't already exist
bool CreateConfigFile(std::string file_name) {
	std::ifstream ifs(file_name.c_str());

	if (!ifs.good() ) { // If the config file doesn't exist
		std::ofstream ofs(file_name.c_str()); // Make it

		ofs.close();
		ifs.close();
		return true; 	// Return true to show that we successfully made the file
	} else {
		ifs.close();
		return false;	// Return false to show we didn't make a file
	}
}

// Writes a property and a value to a file
bool WriteProperty(std::string file_name, std::string prop_name, std::string prop_value) {
	std::ifstream ifs(file_name.c_str());

	if (!ifs.good() ) { // If the config file doesn't exist
		ifs.close();
		return false;
	} else {
		std::ofstream ofs(file_name.c_str(), std::ios_base::app);

		ofs << prop_name + " " + prop_value;

		ofs.close();
		ifs.close();
		return true;
	}
}


std::string ReadProperty(std::string file_name, std::string prop_name) {
	std::ifstream ifs(file_name.c_str());

	if (!ifs.good() ) { // If the config file doesn't exist
		ifs.close();
		return "error";

	} else {
		std::string input;
		std::vector<std::string> terms;

		while (ifs >> input) {
			terms.push_back(input);
		}

		for (int i = 0; i < terms.size(); i++) {
			if (terms.at(i) == prop_name) return terms.at(i+1);
		}

		return "not found";
	}
}
