// CORE
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "renderer.h"
#include "gui.h"
#include "../utils/fileops.h"
#include "../importers/gltf_loader.h"

// MODULES
#include "bx/timer.h"
#include "fmt/format.h"
#include "tiny_gltf.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"

using SimpleEngine::Renderer;

Renderer::Renderer(GUI& gui) : gui(gui) {
}

bgfx::ShaderHandle Renderer::createShader(const std::string& shader, const char* name) {
    const bgfx::Memory* mem = bgfx::copy(shader.data(), shader.size());
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}

void Renderer::setupWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	
    context.width = 800;
    context.height = 600;

    p_window = SDL_CreateWindow(
        "Simple Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        context.width, context.height, SDL_WINDOW_SHOWN);

    assert(p_window != nullptr);

    // EMSCRIPTEN DOES NOT LIKE THIS CHUNK HERE - WHY?
    // TODO, READ THIS: https://wiki.libsdl.org/SDL2/README/emscripten
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(p_window, &wmi);
    assert(SDL_GetWindowWMInfo(p_window, &wmi));

    bgfx::renderFrame();

    bgfx::PlatformData platformData;

#if BX_PLATFORM_WINDOWS
    platformData.ndt = nullptr;
    platformData.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
    platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_LINUX
    platformData.ndt = wmi.info.x11.display;
    platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_EMSCRIPTEN
    platformData.nwh = (void*)"#canvas";
#endif

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = context.width;
    init.resolution.height = context.height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = platformData;

    bgfx::init(init);
    
    bgfx::setDebug(BGFX_DEBUG_TEXT);

}

void Renderer::handleEvents() {
    SDL_Event event;

    if (SDL_PollEvent(&event) > 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {              
                    case SDLK_ESCAPE:
                        active = false;
                        break;
                }

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        active = false;
                        break;
                }
        }
    }
}

void Renderer::findNodes(const std::unique_ptr<Node>& node) {

    // only perform the dynamic_cast in the setup function
    // not each frame
    if (RenderObject* renderObject = dynamic_cast<RenderObject*>(node.get())) {
        renderObjectsFlattened.push_back(renderObject);
    }
    nodesFlattened.push_back(node.get());

    for (const std::unique_ptr<Node>& child : node->children) {
        findNodes(child);
    }
}

void Renderer::flattenSceneGraph() {
    
    // this is the dumb brute force way
    // basically every time we add a model to the scene tree
    // we clear the flattened vectors and repopulate them
    // it's just pointers, but still
    renderObjectsFlattened.clear();
    nodesFlattened.clear();
    
    for (const std::unique_ptr<Node>& scene : scenes) {
        findNodes(scene);
    }
}

void Renderer::setContextVertexLayout() {
    context.layout.begin();
    for (int i = 0; i < vertexLayout.items.size(); i++) {
        const VertexLayoutItem& item = vertexLayout.items[i];
        bool normalized = false;
        if (item.attribute == "COLOR_0") normalized = true;
        context.layout.add(item.bgfxAttrib, item.type, item.bgfxAttribType, normalized);
    }
    context.layout.end();
}

void Renderer::assignBuffers() {
    // this is not super optimized
    for (RenderObject* renderObject : renderObjectsFlattened) {

        // a single bool here is probably inefficient, right?
        if (!renderObject->buffersInitialized) {
            renderObject->vertexBufferHandle = bgfx::createVertexBuffer(
                bgfx::makeRef(&renderObject->mesh.vertexData[0], renderObject->mesh.vertexData.size() * sizeof(uint8_t)),
                context.layout
                );
            
            renderObject->indexBufferHandle = bgfx::createIndexBuffer(
                bgfx::makeRef(&renderObject->mesh.indexData[0], renderObject->mesh.indexData.size() * sizeof(uint16_t))
                );
            renderObject->buffersInitialized = true;
        }
    }
}

// this method might be used externally - inefficeint right now
void Renderer::postImportAddToSceneTree() {
    flattenSceneGraph();
    assignBuffers();
}

void Renderer::setup() {
    setupWindow();

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, context.width, context.height);

    // the big question is WHERE is this layout dictated from?
    // needs alignment between the shader and the geometry
    std::vector<std::string> attributes = { "POSITION", "COLOR_0", "NORMAL", "TEXCOORD_0", "TANGENT" };
    RenderObject::vertexLayoutHelper(vertexLayout, attributes);

    setContextVertexLayout();

    gui.setup(p_window);

    // PIPELINE TESTING
    GltfLoader g("../../assets/gltf/cube_2.gltf", this);
    postImportAddToSceneTree();
    // ---

    std::string vShaderData;
    assert(FileOps::getFileContentsBinary("../shader/b-vertex.bin", vShaderData));
    
    std::string fShaderData;
    assert(FileOps::getFileContentsBinary("../shader/b-fragment.bin", fShaderData));
    
    bgfx::ShaderHandle vShader = createShader(vShaderData, "vShader");
    bgfx::ShaderHandle fShader = createShader(fShaderData, "fShader");
    
    // third argument being true destroys shaders
    context.programHandle = bgfx::createProgram(vShader, fShader, true);

    renderAttribs.timeOffset = bx::getHPCounter();
    renderAttribs.stats = bgfx::getStats();
}

void Renderer::debugRender() {
    bgfx::dbgTextClear();

    renderAttribs.toMsCpu = 1000.0/renderAttribs.stats->cpuTimerFreq;
    bgfx::dbgTextPrintf(0, 2, 0x0f, "cpu time frame %0.3f"
        , double(renderAttribs.stats->cpuTimeFrame)*renderAttribs.toMsCpu
        );
}

void Renderer::update() {

    handleEvents();
    
    gui.update();

    if (!ImGui::GetIO().WantCaptureMouse) {
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
            int delta_x = mouseX - context.prevMouseX;
            int delta_y = mouseY - context.prevMouseY;
            context.camYaw += float(delta_x) * context.rotScale;
            context.camPitch += float(delta_y) * context.rotScale;
        }
        context.prevMouseX = mouseX;
        context.prevMouseY = mouseY;
    }

    renderAttribs.time = (float)((bx::getHPCounter() - renderAttribs.timeOffset) / double(bx::getHPFrequency()));
    
    // TESTING
    double a[3] = {0, 0.2 * bx::sin(10.0 * renderAttribs.time), 0};
    scenes[0]->setTranslation(a, 0b010);
    scenes[0]->updateLocalTransform();
    // ---

    float camRotation[16];
    bx::mtxRotateXYZ(camRotation, context.camPitch, context.camYaw, 0.0f);

    float camTranslation[16];
    bx::mtxTranslate(camTranslation, 0.0f, 0.0f, 5.0f);

    float camTransform[16];
    bx::mtxMul(camTransform, camTranslation, camRotation);

    float view[16];
    bx::mtxInverse(view, camTransform);

    float proj[16];
    bx::mtxProj(
        proj, 60.0f, float(context.width) / float(context.height), 0.1f,
        100.0f, bgfx::getCaps()->homogeneousDepth, bx::Handedness::Right);

    bgfx::setViewTransform(0, view, proj);

    bgfx::touch(0);

    // clean transform pass
    for (Node* node : nodesFlattened) {
        node->cleanTransforms();
    }

    // render pass
    for (RenderObject* renderObject : renderObjectsFlattened) {

        // check that the render object's vertex layout address
        // matches the renderer's vertex layout

        // technically redundant, but there may come a day when this is needed
        if (&renderObject->vertexLayout != &vertexLayout) continue;

        bgfx::setVertexBuffer(0, renderObject->vertexBufferHandle);
        bgfx::setIndexBuffer(renderObject->indexBufferHandle);

        bgfx::setTransform(renderObject->globalTransform);

        uint64_t state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            //| BGFX_STATE_CULL_CW
            //| BGFX_STATE_MSAA
            //| UINT64_C(0)
            ;
        bgfx::setState(state);

        bgfx::submit(0, context.programHandle);
    }

    //debugRender();

    bgfx::frame();
}

void Renderer::cleanup() {
    for (RenderObject* renderObject : renderObjectsFlattened) {
        bgfx::destroy(renderObject->vertexBufferHandle);
        bgfx::destroy(renderObject->indexBufferHandle);
    }

    bgfx::destroy(context.programHandle);

    gui.cleanup();

    bgfx::shutdown();

    SDL_DestroyWindow(p_window);
    SDL_Quit();
}
