#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "fmt/format.h"

#include <string>

#include "model_loader.h"
#include "utils/fileops.h"

void ModelLoader::loadDataFromAttribute(tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute) {

	std::map<std::string, int>::const_iterator pos = primitive.attributes.find(attribute);
	if (pos != primitive.attributes.end()) {
		fmt::println("found attribute {}", attribute);
		int accessorIndex = pos->second;
		const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

		fmt::println("accessor type {}", accessor.type);
		fmt::println("accessor comp type {}", accessor.componentType);

		int bufferViewIndex = accessor.bufferView;
		const tinygltf::BufferView& bufferView = model.bufferViews[bufferViewIndex];

		fmt::println("buffer {}, byte offset {}, byte length {}", bufferView.buffer, bufferView.byteOffset, bufferView.byteLength);
		
		int bufferIndex = bufferView.buffer;
		const tinygltf::Buffer& buffer = model.buffers[bufferIndex];
		
		fmt::println("buffer data size {}", buffer.data.size());
	}
}

void ModelLoader::loadModel() {

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string myFile = "../../assets/gltf/cube.gltf";

	std::string ext = FileOps::getFilePathExtension(myFile);
	
	bool ret = false;
	if (ext.compare("glb") == 0) {
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, myFile.c_str());
	} else {
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, myFile.c_str());
	}

	for (size_t i = 0; i < model.meshes.size(); i++) {
		const tinygltf::Mesh& mesh = model.meshes[i];

		for (size_t j = 0; j < mesh.primitives.size(); j++) {
			const tinygltf::Primitive& primitive = mesh.primitives[i];

			// mode = 4 is triangles
			if (primitive.mode == 4) {
				
				loadDataFromAttribute(model, primitive, "POSITION");
				loadDataFromAttribute(model, primitive, "COLOR_0");

			}
		}

	}

}