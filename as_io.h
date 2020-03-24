#ifndef AS_IO_H
#define AS_IO_H

bool CopyFile(std::string source, std::string dest);

bool CreateConfigFile(std::string file_name);
bool WriteProperty(std::string file_name, std::string prop_name, std::string prop_value);
std::string ReadProperty(std::string file_name, std::string prop_name);

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

#endif
