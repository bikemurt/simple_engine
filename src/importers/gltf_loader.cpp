// SIMPLE_ENGINE
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "gltf_loader.h"
#include "../core/mesh.h"
#include "../utils/fileops.h"

// MODULES
#include "fmt/format.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

GltfLoader::GltfLoader() {
}

void GltfLoader::loadMeshes(const std::string& fileName, std::vector<Mesh>& meshes) {

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string ext = FileOps::getFilePathExtension(fileName);
	
	bool ret = false;
	if (ext.compare("glb") == 0) {
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, fileName.c_str());
	} else {
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, fileName.c_str());
	}

	for (size_t i = 0; i < model.meshes.size(); i++) {
		const tinygltf::Mesh& mesh = model.meshes[i];

		for (size_t j = 0; j < mesh.primitives.size(); j++) {
			const tinygltf::Primitive& primitive = mesh.primitives[j];

			if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
                Mesh renderMesh;

                // this is dumb, but a hack for now
                renderMesh.specifier = std::filesystem::path(fileName).stem().string() + "-" + mesh.name + "-" + std::to_string(j);

                if (std::filesystem::exists(meshImportSavePath + renderMesh.specifier + "-v.bin") &&
                    std::filesystem::exists(meshImportSavePath + renderMesh.specifier + "-i.bin")) {
                    renderMesh.loadFromImportCache();
                } else {
                    processPrimitive(model, primitive, &renderMesh);
                    //renderMesh.saveToImportCache();
                }

                meshes.push_back(std::move(renderMesh));
			}
		}

	}
}

void GltfLoader::findDataFromAttribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData* bufferData) {
    std::map<std::string, int>::const_iterator pos = primitive.attributes.find(attribute);
    if (pos != primitive.attributes.end()) {
        
        int accessorIndex = pos->second;
        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
        
        bufferData->count = accessor.count;
        bufferData->type = accessor.type;
        bufferData->componentType = accessor.componentType;

        int bufferViewIndex = accessor.bufferView;
        const tinygltf::BufferView& bufferView = model.bufferViews[bufferViewIndex];
        
        bufferData->byteStride = accessor.ByteStride(bufferView);

        bufferData->buffer = bufferView.buffer;
        bufferData->offset = bufferView.byteOffset;
        bufferData->length = bufferView.byteLength;
    }
}

void GltfLoader::get8BitFrom16BitUInt(uint8_t* dest, const void* source) {
    uint16_t v_16;

    // a little bit obscure, but works
    // copy 2 bytes from source into v_16
    std::memcpy(&v_16, source, 2);

    // bit shift 8 bits to the right (lose the 8 LSB)
    // then assign to an 8 byte int (dest)
    *dest = v_16 >> 8;
}

void GltfLoader::processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive, Mesh* mesh) {
    
    // assert that the vertex layout is correct and add all buffers
    std::vector<GLTFBufferData> accessorBuffers;
    int vertexByteLength = 0;
    for (int i = 0; i < ATTRIBUTES.size(); i++) {
        GLTFBufferData bufferData;
        findDataFromAttribute(model, primitive, ATTRIBUTES[i].c_str(), &bufferData);
        assert(bufferData.buffer != -1);
        assert(bufferData.type == ATTRIBUTE_TYPES[i]);
        assert(bufferData.componentType == ATTRIBUTE_COMP_TYPES[i]);
        
        accessorBuffers.push_back(std::move(bufferData));
    }


    // get vertex count from the POSITION accessor
    const GLTFBufferData& posBufferDataX = accessorBuffers[1];
    int vertexCount = posBufferDataX.count;

    mesh->vertexData.resize(VERTEX_BYTE_STRIDE * vertexCount);
    for (int i = 0; i < vertexCount; i++) {

        // let's call this the "PACKED" vertex data structure

        // CLR(1) CLR(1) CLR(1) CLR(1)
        // POS(4) POS(4) POS(4)
        // NRM(4) NRM(4) NRM(4)
        // TEX(4) TEX(4)
        // TNG(4) TNG(4) TNG(4) TNG(4)

        // this is the offset WITHIN the interleaved data
        int packedOffsetCounter = 0;

        for (int j = 0; j < ATTRIBUTES.size(); j++) {
            const GLTFBufferData& bufferData = accessorBuffers[j];
            const tinygltf::Buffer& buffer = model.buffers[bufferData.buffer];

            int gltfDataOffset = bufferData.offset + i * bufferData.byteStride;
            int packedDataOffset = packedOffsetCounter + i * VERTEX_BYTE_STRIDE;
            
            if (ATTRIBUTES[j] == "COLOR_0") {
                // structure is a-b-g-r
                for (int k = 0; k < 4; k++) {
                    uint8_t x;
                    // after the gltf data offset, increment by 2 bytes, to get a-b-g-r channels
                    get8BitFrom16BitUInt(&x, &buffer.data[gltfDataOffset + k * 2]);

                    // this part is tricky too - from the packed data offset, we bump up by 1 byte each j index
                    mesh->vertexData[packedDataOffset + k * 1] = x;
                }
                packedOffsetCounter += 4;
            } else {
                std::memcpy(&mesh->vertexData[packedDataOffset], &buffer.data[gltfDataOffset], bufferData.byteStride);
                packedOffsetCounter += bufferData.byteStride;
            }
        }
    }

    // copy the index data
    const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
    const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
    const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];

    // byte length
    mesh->indexData.resize(indicesBufferView.byteLength / 2);
    std::memcpy(&mesh->indexData[0], &indicesBuffer.data[indicesBufferView.byteOffset], indicesBufferView.byteLength);
}

void GltfLoader::vertexLayoutItemHelper(VertexLayoutItem& item, const std::string& attribute) {

    int type = TINYGLTF_TYPE_VEC3;
    int componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;

    if (attribute == "COLOR_0") {
        type = TINYGLTF_TYPE_VEC4;
        componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    } else if (attribute == "TANGENT") {
        type = TINYGLTF_TYPE_VEC4;
    } else if (attribute == "TEXCOORD_0") {
        type = TINYGLTF_TYPE_VEC2;
    }

    item.attribute = attribute;
    item.bgfxAttrib = getBGFXAttribute(attribute);
    item.type = type;
    item.componentType = componentType;
    item.bgfxAttribType = getBGFXAttribType(componentType);

    int bytes = 0;

    // https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#accessor-data-types
    if (componentType == 5120 || componentType == 5121) bytes = 1;
    else if (componentType == 5122 || componentType == 5123) bytes = 2;
    else if (componentType == 5125 || componentType == 5126) bytes = 4;

    item.byteStride = type * bytes;    
}

bgfx::Attrib::Enum GltfLoader::getBGFXAttribute(const std::string& attribute) {
    if (attribute == "POSITION") return bgfx::Attrib::Position;
    else if (attribute == "NORMAL") return bgfx::Attrib::Normal;
    else if (attribute == "TANGENT") return bgfx::Attrib::Tangent;
    else if (attribute == "TEXCOORD_0") return bgfx::Attrib::TexCoord0;
    else if (attribute == "COLOR_0") return bgfx::Attrib::Color0;
    return bgfx::Attrib::Position;
}

bgfx::AttribType::Enum GltfLoader::getBGFXAttribType(int componentType) {
    if (componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) return bgfx::AttribType::Uint8;
    else if (componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) return bgfx::AttribType::Float;
}

void GltfLoader::vertexLayoutHelper(VertexLayout& layout, const std::vector<std::string>& attributes) {
    for (int i = 0; i < attributes.size(); i++) {
        VertexLayoutItem item;
        vertexLayoutItemHelper(item, attributes[i]);
        layout.byteStride += item.byteStride;
        layout.items.push_back(std::move(item));
    }
}