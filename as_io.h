/* IO functions specially made for SaveGuard
 * Written by Aaron Sigal
 *
 *
*/

#ifndef AS_IO_H
#define AS_IO_H

// File IO
bool CopyFile(const std::string &source, const std::string &dest);
bool CreateConfigFile(const std::string &file_name, const std::string &path);

// Property IO
bool WriteProperty(const std::string &file_name, const std::string &prop_name, const std::string &prop_value);
std::string ReadProperty(const std::string &file_name, const std::string &prop_name);

// Console IO
std::string log(const std::string &text);
std::string err(const std::string &text);

bool CopyFile(const std::string &source, const std::string &dest) {
>>>>>>> dev_feature_better_log
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

// Generates an empty config file if one doesn't already exist
bool CreateConfigFile(const std::string &file_name, const std::string &path) {
	std::ifstream ifs( (path + file_name).c_str());

	if (!ifs.good() ) { // If the config file doesn't exist
		std::ofstream ofs((path + file_name).c_str()); // Make it

		ofs.close();
		ifs.close();
		return true; 	// Return true to show that we successfully made the file
	} else {
		ifs.close();
		return false;	// Return false to show we didn't make a file
	}
}

// Writes a property and a value to a file
bool WriteProperty(const std::string &file_name, const std::string &prop_name, const std::string &prop_value) {
	std::ifstream ifs(file_name.c_str());

	if (!ifs.good() ) { // If the config file doesn't exist
		ifs.close();
		return false;
	} else {
		std::ofstream ofs(file_name.c_str(), std::ios_base::app);

		ofs << prop_name + " " + prop_value << std::endl;

		ofs.close();
		ifs.close();
		return true;
	}
}


// Reads a property and returns it
std::string ReadProperty(const std::string &file_name, const std::string &prop_name) {

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

std::string log(const std::string &text, std:time_t tt) {

}
std::string err(const std::string &text) {

}

#endif
