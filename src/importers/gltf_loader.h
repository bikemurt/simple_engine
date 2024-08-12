#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include <string>
#include <vector>
#include <array>

#include "../core/renderer.h"
#include "../core/render_object.h"
#include "../core/mesh.h"
#include "../utils/fileops.h"
#include "tiny_gltf.h"

class GltfLoader {

private:

    std::string meshImportSavePath = "C:\\Projects\\CppTesting\\simple_engine\\imports\\meshes\\";

    const VertexLayout& vertexLayout;
    std::string fileName;

    void processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive, Mesh* renderMesh);

    struct GLTFBufferData {
        size_t count = 0;
        int type = -1;
        int componentType = -1;
        int byteStride = 0;
        int buffer = -1;
        size_t offset = 0;
        size_t length = 0;
    };
    void findDataFromAttribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData* bufferData);
    void get8BitFrom16BitUInt(uint8_t* dest, const void* source);

public:
    
    GltfLoader(std::string fileName, const VertexLayout& vertexLayout);
    void loadMeshes(std::vector<Mesh>& meshes);

};

#endif