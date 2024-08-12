#ifndef FILEOPS_H
#define FILEOPS_H

#include <cassert>
#include <string>

namespace FileOps {
    
    std::string getFilePathExtension(const std::string &fileName);

    bool getFileContentsBinary(const char *filePath, std::string &fileContents);

}

#endif