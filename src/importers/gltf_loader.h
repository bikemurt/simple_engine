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

    const VertexLayout& vertexLayout;
    std::string fileName;
    
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;

    void processPrimitive(const tinygltf::Primitive& primitive, Mesh& renderMesh);

    struct GLTFBufferData {
        size_t count = 0;
        int type = -1;
        int componentType = -1;
        int byteStride = 0;
        int buffer = -1;
        size_t offset = 0;
        size_t length = 0;
    };
    void findDataFromAttribute(const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData& bufferData);
    void convert16BitUintTo8Bit(uint8_t* dest, const void* source);
    void processNode(const tinygltf::Node& gltfNode, Node& node);

public:
    
    GltfLoader(std::string fileName, const VertexLayout& vertexLayout);
    void loadMeshes(std::vector<Mesh>& meshes);
    void loadScenes(std::vector<Node>& scenes);

};

#endif