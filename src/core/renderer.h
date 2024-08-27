#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"
#include "bx/math.h"

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "tiny_gltf.h"

#include "node.h"
#include "render_object.h"
#include "mesh.h"

namespace SimpleEngine {

class Renderer {

private:

    SDL_Window* p_window;
    SDL_SysWMinfo wmi;

    struct Context {
        uint32_t width = 800;
        uint32_t height = 600;

        bgfx::VertexLayout layout;
        bgfx::ProgramHandle programHandle;

        float camPitch = 0.0f;
        float camYaw = 0.0f;
        float rotScale = 0.01f;

        int prevMouseX = 0;
        int prevMouseY = 0;

    } context;

    int64_t timeOffset;

    std::vector<RenderObject*> renderObjects;
    std::vector<Mesh> meshes;
    std::vector<std::unique_ptr<Node>> scenes;

    void setContextVertexLayout(const VertexLayout& vertexLayout);

    void setupWindow();
    void handleEvents();
    void cleanupWindow();
        
    bgfx::ShaderHandle createShader(const std::string& shader, const char* name);

    void processScenes();
    void findRenderObjects(const std::unique_ptr<Node>& node);

public:

    bool active = true;

    Renderer();

    void setup();
    void renderFrame();
    void cleanup();

};

}


#endif