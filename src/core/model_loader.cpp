#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "model_loader.h"

std::string ModelLoader::get_file_path_extension(const std::string &fileName) {
	if (fileName.find_last_of(".") != std::string::npos) {
		return fileName.substr(fileName.find_last_of(".") + 1);
	}
	return "";
}

void ModelLoader::load_model() {

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string myFile = "C:/Projects/CppTesting/test4/assets/gltf/cube.gltf";

	std::string ext = get_file_path_extension(myFile);
	
	bool ret = false;
	if (ext.compare("glb") == 0) {
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, myFile.c_str());
	} else {
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, myFile.c_str());
	}
}