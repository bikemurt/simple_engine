#ifndef MATERIAL_H
#define MATERIAL_H

#include "bgfx/bgfx.h"

#include <string>
#include <vector>

#include "texture.h"

class Material {

public:

    std::string vertexShader;
    std::string fragShader;
    bgfx::VertexLayout vertexLayout;
    std::vector<Texture> textures;

    struct Uniform {
        std::string name;
        bgfx::UniformType::Enum type;
    };

    std::vector<Uniform> uniforms;

};

#endif