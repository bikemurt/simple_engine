#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

// CORE
#include "node.h"
#include "mesh.h"
#include "material.h"

// MODULES
#include "bgfx/bgfx.h"

namespace SimpleEngine {

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

class RenderObject : public Node {
private:
    static void vertexLayoutItemHelper(VertexLayoutItem& item, const std::string& attribute);
    static bgfx::Attrib::Enum getBGFXAttribute(const std::string& attribute);
    static bgfx::AttribType::Enum getBGFXAttribType(int componentType);

public:
    bgfx::VertexBufferHandle vertexBufferHandle;
    bgfx::IndexBufferHandle indexBufferHandle;

    const VertexLayout& vertexLayout;
    
    const Mesh& mesh;
    //const Material& material;

    RenderObject(const Mesh& mesh, const VertexLayout& vertexLayout);
    static void vertexLayoutHelper(VertexLayout& layout, const std::vector<std::string>& attributes);
    
};

}

#endif