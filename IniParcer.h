#pragma once

#ifndef INI_PARCER_
#define INI_PARCER_


#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <utility> 


class IniParcer
{
public:
	IniParcer(const std::string& iniFilePath);
	~IniParcer();

	std::map<std::string, std::string> ReadIniFile(const std::string& filename);
	std::string TrimString(const std::string& str);
	int ConvertPortIntoInt(const std::string& stringport);

	std::pair<std::string, int> Run();

private:
	std::string strPathToIniFile;
};


#endif
