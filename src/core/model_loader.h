#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>

class ModelLoader {

private:
    
    std::string get_file_path_extension(const std::string &fileName);

public:

    void load_model();

};

#endif