#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include <string>
#include <vector>
#include <array>

#include "../core/renderer.h"
#include "../core/mesh.h"
#include "../utils/fileops.h"
#include "tiny_gltf.h"

struct VertexLayoutItem {
    std::string attribute;
    bgfx::Attrib::Enum bgfxAttrib;

    int type;
    
    int componentType;
    bgfx::AttribType::Enum bgfxAttribType;
    
    int byteStride;
};

struct VertexLayout {
    std::vector<VertexLayoutItem> items;
    int byteStride;
};

class GltfLoader {

private:

    std::string meshImportSavePath = "C:\\Projects\\CppTesting\\simple_engine\\imports\\meshes\\";

    // used for attribute validation from Blender
    const std::array<std::string, 5> ATTRIBUTES = {
        "POSITION",
        "COLOR_0",
        "NORMAL",
        "TEXCOORD_0",
        "TANGENT"
        };
    const std::array<int, 5> ATTRIBUTE_TYPES = {
        TINYGLTF_TYPE_VEC3,
        TINYGLTF_TYPE_VEC4,
        TINYGLTF_TYPE_VEC3,
        TINYGLTF_TYPE_VEC2,
        TINYGLTF_TYPE_VEC4
        };
    const std::array<int, 5> ATTRIBUTE_COMP_TYPES = {
        TINYGLTF_COMPONENT_TYPE_FLOAT,
        TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT,
        TINYGLTF_COMPONENT_TYPE_FLOAT,
        TINYGLTF_COMPONENT_TYPE_FLOAT,
        TINYGLTF_COMPONENT_TYPE_FLOAT
        };
    
    // final vertex stride - note that color is compacted down to 1 byte per channel (ABGR)
    const int VERTEX_BYTE_STRIDE = 4*1 + 3*4 + 3*4 + 2*4 + 4*4;

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
    
    static void vertexLayoutItemHelper(VertexLayoutItem& item, const std::string& attribute);
    static bgfx::Attrib::Enum getBGFXAttribute(const std::string& attribute);
    static bgfx::AttribType::Enum getBGFXAttribType(int componentType);

public:
    
    GltfLoader();
    void loadMeshes(const std::string& fileName, std::vector<Mesh>& meshes);
    static void vertexLayoutHelper(VertexLayout& layout, const std::vector<std::string>& attributes);

};

#endif