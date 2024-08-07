#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "tiny_gltf.h"
#include "bgfx.h"

class Mesh {

private:
    std::vector<uint8_t> vertexData;
    std::vector<uint16_t> indexData;

    struct LayoutItem {
        bgfx::Attrib attrib;
        uint8_t components;
        bgfx::AttribType attribType;
    };

    std::vector<LayoutItem> layout;

    struct GLTFBufferData {
        size_t count = 0;
        int type = -1;
        int byteStride = 0;
        int buffer = -1;
        size_t offset = 0;
        size_t length = 0;
    };
    void findDataFromAttribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData* bufferData);
    void get8BitFrom16BitUInt(uint8_t* dest, const void* source);

public:
    void loadModel(const std::string& filename);

    void saveModel(const std::string& filename);

};

#endif