// core includes
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "render_object.h"
#include "tiny_gltf.h"

RenderObject::RenderObject(const Mesh& mesh, const VertexLayout& vertexLayout) :
    mesh(mesh), vertexLayout(vertexLayout) {

}


void RenderObject::vertexLayoutItemHelper(VertexLayoutItem& item, const std::string& attribute) {

    int type = TINYGLTF_TYPE_VEC3;
    int componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;

    if (attribute == "COLOR_0") {
        type = TINYGLTF_TYPE_VEC4;
        componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
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

bgfx::Attrib::Enum RenderObject::getBGFXAttribute(const std::string& attribute) {
    if (attribute == "POSITION") return bgfx::Attrib::Position;
    else if (attribute == "NORMAL") return bgfx::Attrib::Normal;
    else if (attribute == "TANGENT") return bgfx::Attrib::Tangent;
    else if (attribute == "TEXCOORD_0") return bgfx::Attrib::TexCoord0;
    else if (attribute == "COLOR_0") return bgfx::Attrib::Color0;
    return bgfx::Attrib::Position;
}

bgfx::AttribType::Enum RenderObject::getBGFXAttribType(int componentType) {
    if (componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) return bgfx::AttribType::Uint8;
    else if (componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) return bgfx::AttribType::Float;
}

void RenderObject::vertexLayoutHelper(VertexLayout& layout, const std::vector<std::string>& attributes) {
    layout.byteStride = 0;
    for (int i = 0; i < attributes.size(); i++) {
        VertexLayoutItem item;
        vertexLayoutItemHelper(item, attributes[i]);
        layout.byteStride += item.byteStride;
        layout.items.push_back(std::move(item));
    }
}