#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>

#include "tiny_gltf.h"

class ModelLoader {

private:
    void loadDataFromAttribute(tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute);

public:

    void loadModel();

};

#endif