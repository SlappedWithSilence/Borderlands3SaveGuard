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

#include "as_io.h"

//Forward declerations

int BackupSaves();
bool LaunchBorderlands();
bool startup();


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

// Default Functionality Constants
int TIME_TO_BACKUP      = 10 * 60; // Amount of time the program will wait before calling BackupSaves().
int MAX_NUMBER_OF_SAVES = 5;
int NUMBER_OF_BACKUPS   = 1;

int main() {

	if ( !startup() ) {
		std::string i;
		std::cin >> i; 
		return -1;     // If startup fails for any reason, exit the program
	}

	auto last_backup = std::time(0); // I initialize the backup timestamp to the current time.

	while(true) {					 // Infinite loop. Doesn't end until the user closes to program.
		if (std::time(0) - last_backup >= TIME_TO_BACKUP) {
			last_backup = std::time(0);;

			std::cout <<  "Backing up...";
			std:: cout << "Total number of characters backed up: " << BackupSaves() << std::endl;
			std:: cout << " done!" << std::endl;
		}
	}
}

bool startup() {
	// Intro Prompt
	std::cout << "Please note, this program will not run correctly if it is not inside your Borderlands 3 save folder." << std::endl;
	std::cout << "If it isn't, please close this window and move it now." << std::endl;

	//Ensure that the folder tree exists
	mkdir(ROOT_FOLDER_PREFIX.c_str());
	mkdir(CONFIG_FOLDER_PREFIX.c_str());


	// Generate default config file
	if ( CreateConfigFile(CONFIG_FILE_NAME, CONFIG_FOLDER_PREFIX) ) { // If there was no config file previously
 
		// Write default values to the config file
		WriteProperty(CONFIG_FOLDER_PREFIX + CONFIG_FILE_NAME, "save_period",       std::to_string(TIME_TO_BACKUP)      );
		WriteProperty(CONFIG_FOLDER_PREFIX + CONFIG_FILE_NAME, "number_of_saves",   std::to_string(MAX_NUMBER_OF_SAVES) );
		WriteProperty(CONFIG_FOLDER_PREFIX + CONFIG_FILE_NAME, "number_of_backups", std::to_string(NUMBER_OF_BACKUPS)   );

	} else {
		std::cout << "Props loaded: " << std::endl;
		TIME_TO_BACKUP      = std::stoi(ReadProperty(CONFIG_FOLDER_PREFIX + CONFIG_FILE_NAME, "save_period"));       
		MAX_NUMBER_OF_SAVES = std::stoi(ReadProperty(CONFIG_FOLDER_PREFIX + CONFIG_FILE_NAME, "number_of_saves")); 
		NUMBER_OF_BACKUPS   = std::stoi(ReadProperty(CONFIG_FOLDER_PREFIX + CONFIG_FILE_NAME, "number_of_backups"));
	}

	// Make backup folders
	mkdir(BACKUP_FOLDER_PREFIX.c_str());
	for (int i = 0; i < NUMBER_OF_BACKUPS; i++) {
		mkdir((BACKUP_FOLDER_PREFIX + "/" + std::to_string(i)).c_str());
	}

	std:: cout << "Total number of characters backed up: " << BackupSaves() << std::endl; // Make an initial backup before we run anything

	// Try to launch Borderlands. If it doesn't work, exit the function (and ideally the entire program)
	if (!LaunchBorderlands()) {
		std::cout << BORDERLANDS_LAUNCH_FAILED;
		//return false;
	}

	return true;
}

void RootFolderBackup(int &successes) {
	for (int i = 0; i < MAX_NUMBER_OF_SAVES; ++i) {
		std::string file_name = std::to_string(i) + SAVE_EXTENSION;

		std::ifstream ifs;
		ifs.open(file_name);

		if(ifs.good()) {
			ifs.close();

			std::string location = BACKUP_FOLDER_PREFIX + "0/" + file_name;

			if (CopyFile(file_name, location ) ) successes++;

		} else {
			ifs.close();
			std::cout << "\nCouldn't locate " << file_name << "!\n";
		}
	}

	CopyFile(PROFILE_FILE_NAME, BACKUP_FOLDER_PREFIX + PROFILE_FILE_NAME); // Backup the profile save file
}

// Backs up all relevent save files.
int BackupSaves() {
	int successes = 0;
	if (NUMBER_OF_BACKUPS == 1) { // Just one backup
		RootFolderBackup(successes);

	} else if (NUMBER_OF_BACKUPS > 1) { // Maintain multiple backups
		for (int j = NUMBER_OF_BACKUPS - 1; j >= 1; --j) {

			for (int i = 0; i < MAX_NUMBER_OF_SAVES; ++i) {
				std::string file_name =  std::to_string(i) + SAVE_EXTENSION;
				std::string file_location = BACKUP_FOLDER_PREFIX + std::to_string(j - 1) + "/";


				std::ifstream ifs;
				ifs.open(file_location + file_name);

				if(ifs.good()) {
					ifs.close();

					std::string file_destination = BACKUP_FOLDER_PREFIX + std::to_string(j) + "/" + file_name;

					if (CopyFile(file_location + file_name, file_destination ) ) successes++;

				} else {
					ifs.close();
					std::cout << "\nCouldn't locate " << file_name << " for backup #" + std::to_string(j) + "\n";
				}
			}
		}

		RootFolderBackup(successes);

	} else {
		// Clearly something went wrong, 
		return -1;
	}

	

	return successes;
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
