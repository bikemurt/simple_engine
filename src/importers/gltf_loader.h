#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include <memory>
#include <string>
#include <vector>
#include <array>

#include "../core/renderer.h"
#include "../core/render_object.h"
#include "../core/mesh.h"
#include "../core/node.h"
#include "../utils/fileops.h"
#include "tiny_gltf.h"

namespace SimpleEngine {

class Renderer;

class GltfLoader {

private:

    Renderer* p_renderer;

    std::string fileName;
    
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;

    struct GLTFBufferData {
        size_t count = 0;
        int type = -1;
        int componentType = -1;
        int byteStride = 0;
        int buffer = -1;
        size_t offset = 0;
        size_t length = 0;
    };

    // is this vector massive?? we are mapping to another std::vector index, so size_t seems right
    std::vector<size_t> globalMeshMap;

    void processPrimitive(const tinygltf::Primitive& primitive, Mesh& renderMesh);

    void findDataFromAttribute(const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData& bufferData);
    void convert16BitUintTo8Bit(uint8_t* dest, const void* source);
    void processNode(const tinygltf::Node& gltfNode, std::unique_ptr<Node>& node, Node* parentNode);
    void loadMeshes();
    void loadScenes();

public:
    
    GltfLoader(std::string fileName, Renderer* p_renderer);

};

}

#endif