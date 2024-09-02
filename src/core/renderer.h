#ifndef RENDERER_H
#define RENDERER_H

// CORE
#include "node.h"
#include "render_object.h"
#include "mesh.h"
#include "gui.h"

// MODULES
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"
#include "bx/math.h"

// STDLIB
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "tiny_gltf.h"

namespace SimpleEngine {

class Renderer {

friend class GltfLoader;
friend class GUI;

private:
    // this is consumed by the GUI
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

    struct RenderAttribs {
        int64_t timeOffset;
        const bgfx::Stats* stats;
        float time;
        double toMsCpu;
    } renderAttribs;

    // these are public now both for the gltf_loader and GUI
    VertexLayout vertexLayout;
    std::vector<Mesh> meshes;
    std::vector<std::unique_ptr<Node>> scenes;

    // gui is injected because the initialization order matters
    // and is buried in the renderer setup/update functions
    GUI& gui;

    // flat render structure
    std::vector<Node*> nodesFlattened;
    std::vector<RenderObject*> renderObjectsFlattened;

    void setContextVertexLayout();

    void setupWindow();
    void handleEvents();
    
    bgfx::ShaderHandle createShader(const std::string& shader, const char* name);

    void flattenSceneGraph();
    void findNodes(const std::unique_ptr<Node>& node);

    void debugRender();
    void assignBuffers();

    void postImportAddToSceneTree();
    void renderPass();
    void cameraViewUpdate();

public:

    bool active = true;

    Renderer(GUI& gui);

    void setup();
    void update();
    void cleanup();

};

}


#endif