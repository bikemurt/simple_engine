
#include "fileops.h"

#include <fstream>
#include <iostream>
#include <string>

std::string FileOps::getFilePathExtension(const std::string &fileName) {
	if (fileName.find_last_of(".") != std::string::npos) {
		return fileName.substr(fileName.find_last_of(".") + 1);
	}
	return "";
}

bool FileOps::getFileContentsBinary(const char* filePath, std::string& fileContents) {
    std::ifstream stream(filePath, std::ios_base::binary);
    
    if (!stream.is_open()) return false;
    
    stream.seekg(0, std::ios::end);

    std::streampos endPos = stream.tellg();
    if (endPos == -1) return false;

    fileContents.resize(endPos);

    stream.seekg(0, std::ios::beg);
    stream.read(&fileContents[0], fileContents.size());
    
    stream.close();

    return true;
}