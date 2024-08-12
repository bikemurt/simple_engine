#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "bgfx/bgfx.h"

#include "node.h"
#include "mesh.h"
#include "material.h"

class RenderObject : public Node {

public:
    bgfx::VertexBufferHandle vertexBufferHandle;
    bgfx::IndexBufferHandle indexBufferHandle;

    const Mesh& mesh;
    //const Material& material;

    RenderObject(const Mesh& m);
};

#endif