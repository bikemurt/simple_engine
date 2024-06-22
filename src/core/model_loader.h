#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>

class ModelLoader {

private:
    
    std::string getFilePathExtension(const std::string &fileName);

public:

    void loadModel();

};

#endif