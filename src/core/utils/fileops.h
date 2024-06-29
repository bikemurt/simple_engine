#ifndef FILEOPS_H
#define FILEOPS_H

#include <cassert>
#include <string>

namespace FileOps {

bool getFileContentsBinary(const char* filePath, std::string& fileContents);

}

#endif