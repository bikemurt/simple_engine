// core includes
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "renderer.h"
#include "../utils/fileops.h"

#include "../importers/gltf_loader.h"

// module includes
#include "fmt/format.h"

Renderer::Renderer() {
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

    // TODO: platform specific stuff here
    p_window = SDL_CreateWindow(
        "Simple Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        context.width, context.height, SDL_WINDOW_SHOWN);

    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(p_window, &wmi);
}

void Renderer::handleEvents() {
    SDL_Event event;

    if (SDL_PollEvent(&event) > 0) {

        switch (event.type) {
            case SDL_KEYDOWN:
                case SDLK_ESCAPE:
                    active = false;
                    break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        active = false;
                        break;
                }
        }
    }
}

void Renderer::cleanupWindow() {
    SDL_DestroyWindow(p_window);
    SDL_Quit();
}

void Renderer::setup() {
    setupWindow();

    bgfx::renderFrame();

    bgfx::PlatformData platformData;
    platformData.ndt = nullptr;
    platformData.nwh = wmi.info.win.window;

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = context.width;
    init.resolution.height = context.height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = platformData;

    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, context.width, context.height);

    // PIPELINE TESTING
    GltfLoader g;
    g.loadMeshes("../../assets/gltf/cube_2.gltf", meshes);

    VertexLayout vertexLayout;
    std::vector<std::string> attributes = { "POSITION", "COLOR_0", "NORMAL", "TEXCOORD_0", "TANGENT" };
    GltfLoader::vertexLayoutHelper(vertexLayout, attributes);

    context.layout.begin();
    for (int i = 0; i < vertexLayout.items.size(); i++) {
        VertexLayoutItem& item = vertexLayout.items[i];
        bool normalized = false;
        if (item.attribute == "COLOR_0") normalized = true;
        context.layout.add(item.bgfxAttrib, item.type, item.bgfxAttribType, normalized);
    }
    context.layout.end();

/*
    context.layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Float)
        .end();*/

    // do some magic for now, creating a whole bunch of renderObjects with that mesh
    for (int i = 0; i < meshes.size(); i++) {
        // hopefully not doing anything dumb here
        // this should keep the "mesh" ownership in the meshes vector
        renderObjects.push_back(RenderObject(meshes[i]));
    }

    // FROM HERE FORWARD SHOULD NOT CHANGE TOO MUCH
    for (int i = 0; i < renderObjects.size(); i++) {
        renderObjects[i].vertexBufferHandle = bgfx::createVertexBuffer(
            bgfx::makeRef(&renderObjects[i].mesh.vertexData[0], renderObjects[i].mesh.vertexData.size() * sizeof(uint8_t)),
            context.layout
            );
        
        renderObjects[i].indexBufferHandle = bgfx::createIndexBuffer(
            bgfx::makeRef(&renderObjects[i].mesh.indexData[0], renderObjects[i].mesh.indexData.size() * sizeof(uint16_t))
            );
    }

    std::string vShaderData;
    assert(FileOps::getFileContentsBinary("../shader/vertex.bin", vShaderData));
    
    std::string fShaderData;
    assert(FileOps::getFileContentsBinary("../shader/fragment.bin", fShaderData));
    
    bgfx::ShaderHandle vShader = createShader(vShaderData, "vShader");
    bgfx::ShaderHandle fShader = createShader(fShaderData, "fShader");
    
    // third argument being true destroys shaders
    context.programHandle = bgfx::createProgram(vShader, fShader, true);

}

void Renderer::renderFrame() {

    handleEvents();
    
    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
        int delta_x = mouseX - context.prevMouseX;
        int delta_y = mouseY - context.prevMouseY;
        context.camYaw += float(-delta_x) * context.rotScale;
        context.camPitch += float(-delta_y) * context.rotScale;
    }
    context.prevMouseX = mouseX;
    context.prevMouseY = mouseY;

    float camRotation[16];
    bx::mtxRotateXYZ(camRotation, context.camPitch, context.camYaw, 0.0f);

    float camTranslation[16];
    bx::mtxTranslate(camTranslation, 0.0f, 0.0f, -5.0f);

    float camTransform[16];
    bx::mtxMul(camTransform, camTranslation, camRotation);

    float view[16];
    bx::mtxInverse(view, camTransform);

    float proj[16];
    bx::mtxProj(
        proj, 60.0f, float(context.width) / float(context.height), 0.1f,
        100.0f, bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

    bgfx::touch(0);

    float model[16];
    bx::mtxIdentity(model);
    bgfx::setTransform(model);

    for (int i = 0; i < renderObjects.size(); i++) {
        bgfx::setVertexBuffer(0, renderObjects[i].vertexBufferHandle);
        bgfx::setIndexBuffer(renderObjects[i].indexBufferHandle);

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

    bgfx::frame();
}

void Renderer::cleanup() {
    for (int i = 0; i < renderObjects.size(); i++) {
        bgfx::destroy(renderObjects[i].vertexBufferHandle);
        bgfx::destroy(renderObjects[i].indexBufferHandle);
    }

    bgfx::destroy(context.programHandle);

    bgfx::shutdown();

}
