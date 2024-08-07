#include "mesh.h"

#include "utils/fileops.h"

void Mesh::findDataFromAttribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData* bufferData) {
    std::map<std::string, int>::const_iterator pos = primitive.attributes.find(attribute);
    if (pos != primitive.attributes.end()) {
        
        int accessorIndex = pos->second;
        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
        
        bufferData->count = accessor.count;
        bufferData->type = accessor.type;

        int bufferViewIndex = accessor.bufferView;
        const tinygltf::BufferView& bufferView = model.bufferViews[bufferViewIndex];
        
        (*bufferData).byteStride = accessor.ByteStride(bufferView);

        bufferData->buffer = bufferView.buffer;
        bufferData->offset = bufferView.byteOffset;
        bufferData->length = bufferView.byteLength;
    }
}

void Mesh::get8BitFrom16BitUInt(uint8_t* dest, const void* source) {
    uint16_t v_16;
    std::memcpy(&v_16, source, 2);
    *dest = v_16 >> 8;
}

void Mesh::loadModel(const std::string& filename) {

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string ext = FileOps::getFilePathExtension(filename);
	
	bool ret = false;
	if (ext.compare("glb") == 0) {
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename.c_str());
	} else {
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename.c_str());
	}

	for (size_t i = 0; i < model.meshes.size(); i++) {
		const tinygltf::Mesh& mesh = model.meshes[i];

		for (size_t j = 0; j < mesh.primitives.size(); j++) {
			const tinygltf::Primitive& primitive = mesh.primitives[i];

			// mode = 4 is triangles
			if (primitive.mode == 4) {
				
                GLTFBufferData posBufferData;
                findDataFromAttribute(model, primitive, "POSITION", &posBufferData);

                GLTFBufferData clrBufferData;
                findDataFromAttribute(model, primitive, "COLOR_0", &clrBufferData);

                assert(posBufferData.buffer != -1);
                assert(posBufferData.type == TINYGLTF_TYPE_VEC3);

                assert(clrBufferData.buffer != -1);
                assert(clrBufferData.type == TINYGLTF_TYPE_VEC4);

                int vertexCount = posBufferData.length / posBufferData.byteStride;

                // division by 2 to go from uint16_t to uint8_t
                vertexData.resize(posBufferData.length + clrBufferData.length / 2);

                const tinygltf::Buffer& clrBuffer = model.buffers[clrBufferData.buffer];
                const tinygltf::Buffer& posBuffer = model.buffers[posBufferData.buffer];

                // position data = 3 floats * 4 bytes per float = 12 bytes
                // R,G,B,A one byte each = 4 bytes
                int fullByteStride = posBufferData.byteStride + clrBufferData.byteStride / 2;
                for (int i = 0; i < vertexCount; i++) {

                    // first 3*4 = 12 bytes
                    std::memcpy(&vertexData[i * fullByteStride + 0],
                        &posBuffer.data[posBufferData.offset + i * posBufferData.byteStride], posBufferData.byteStride);

                    // each color channel (RGBA) has to be converted from 16 bits to 8 bits
                    int clrOffset = clrBufferData.offset + i * clrBufferData.byteStride;

                    uint8_t a;
                    get8BitFrom16BitUInt(&a, &clrBuffer.data[clrOffset + 0]);
                    
                    uint8_t b;
                    get8BitFrom16BitUInt(&b, &clrBuffer.data[clrOffset + 2]);
                    
                    uint8_t g;
                    get8BitFrom16BitUInt(&g, &clrBuffer.data[clrOffset + 4]);
                    
                    uint8_t r;
                    get8BitFrom16BitUInt(&r, &clrBuffer.data[clrOffset + 6]);
                    
                    // next 4*1 = 4 bytes
                    // structure is abgr
                    vertexData[i * fullByteStride + 12] = a;
                    vertexData[i * fullByteStride + 13] = b;
                    vertexData[i * fullByteStride + 14] = g;
                    vertexData[i * fullByteStride + 15] = r;

                }

                // indices should(?) be straightforward
                const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];

                indexData.resize(indicesBufferView.byteLength / 2);
                std::memcpy(&indexData[0], &indicesBuffer.data[indicesBufferView.byteOffset], indicesBufferView.byteLength);
                
			}
		}

	}
}