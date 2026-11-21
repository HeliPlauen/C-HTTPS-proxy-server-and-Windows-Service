#include "IniParcer.h"

#include <utility>


IniParcer::IniParcer(const std::string & iniFilePath) {
    this->strPathToIniFile = iniFilePath;

    std::cout << "CONSTRUCTOR INI-PARCER" << std::endl;
}


IniParcer::~IniParcer() {
    std::cout << "DESTRUCTOR INI-PARCER" << std::endl;
}


// Function to read the INI file and return the data as a map
std::map<std::string, std::string> IniParcer::ReadIniFile(const std::string& filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> data;
    std::string line;
    std::string section;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            line = this->TrimString(line);

            // Skip comments and empty lines
            if (line.empty() || line[0] == ';' || line[0] == '#') continue;

            // Check if it's a section
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                section = line.substr(1, line.size() - 2);
            }
            // Otherwise, it's a key-value pair
            else {
                size_t equalsPos = line.find('=');
                if (equalsPos != std::string::npos) {
                    std::string key = this->TrimString(line.substr(0, equalsPos));
                    std::string value = this->TrimString(line.substr(equalsPos + 1));
                    data[section + "." + key] = value;  // Store section.key as the map key
                }
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return data;
}


// Function to trim whitespace from both ends of a string
std::string IniParcer::TrimString(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}


int IniParcer::ConvertPortIntoInt(const std::string& stringport) {
    int port = atoi(stringport.c_str());
    if (port) {
        return port;
    }
    else {
        throw std::runtime_error("Uncorrect data!");
    }
}


std::pair<std::string, int> IniParcer::Run() {
    std::string url;

    std::string port;
    int intPort = 0;

    std::string defaultUrl = "127.0.0.1";
    int defaultPort = 8889;

    std::map<std::string, std::string> config = this->ReadIniFile(this->strPathToIniFile);

    try {
        port = config["Main.access_port"];
        if (port == "") {
            throw std::runtime_error("Data was not gotten!");
        }

        url = config["Main.target_url"];
        intPort = this->ConvertPortIntoInt(port);

        return { url, intPort };
    }
    catch (std::runtime_error& ex1) {
        std::cout << "Error: " << ex1.what() << std::endl;
        return { defaultUrl, defaultPort };
    }
}
