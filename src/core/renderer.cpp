#define ENABLE_DEBUG
#include "utils/debug.h"

#include "renderer.h"
#include "fmt/format.h"
#include "utils/fileops.h"

#include <string>

Renderer::Renderer() {
}


static Renderer::PosColorVertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};


static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

bgfx::ShaderHandle Renderer::createShader(const std::string& shader, const char* name) {
    const bgfx::Memory* mem = bgfx::copy(shader.data(), shader.size());
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}

void Renderer::setup() {

	SDL_Init(SDL_INIT_VIDEO);
	
    m_context.width = 800;
    m_context.height = 600;

    // TODO: platform specific stuff here
    p_window = SDL_CreateWindow(
        "Simple Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_context.width, m_context.height, SDL_WINDOW_SHOWN);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(p_window, &wmi);

    bgfx::renderFrame();

    bgfx::PlatformData platformData;
    platformData.ndt = nullptr;
    platformData.nwh = wmi.info.win.window;

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = m_context.width;
    init.resolution.height = m_context.height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = platformData;

    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, m_context.width, m_context.height);

    // configure vertex layout in the GPU for indexed rendering of the triangles
    m_context.layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    
    m_context.vertexBufferHandle = bgfx::createVertexBuffer(
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        m_context.layout
        );

    m_context.indexBufferHandle = bgfx::createIndexBuffer(
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
        );

    std::string vShaderData;
    assert(FileOps::getFileContentsBinary("../shader/vertex.bin", vShaderData));
    
    std::string fShaderData;
    assert(FileOps::getFileContentsBinary("../shader/fragment.bin", fShaderData));
    
    bgfx::ShaderHandle vShader = createShader(vShaderData, "vShader");
    bgfx::ShaderHandle fShader = createShader(fShaderData, "fShader");
    
    // third argument being true destroys shaders
    m_context.programHandle = bgfx::createProgram(vShader, fShader, true);

}

void Renderer::handleEvents() {
    SDL_Event event;

    if (SDL_PollEvent(&event) > 0) {

        switch (event.type) {
            case SDL_KEYDOWN:
                case SDLK_ESCAPE:
                    m_active = false;
                    break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        m_active = false;
                        break;
                }
        }
    }
}

void Renderer::renderFrame() {

    handleEvents();
    
    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
        int delta_x = mouseX - m_context.prevMouseX;
        int delta_y = mouseY - m_context.prevMouseY;
        m_context.camYaw += float(-delta_x) * m_context.rotScale;
        m_context.camPitch += float(-delta_y) * m_context.rotScale;
    }
    m_context.prevMouseX = mouseX;
    m_context.prevMouseY = mouseY;

    float camRotation[16];
    bx::mtxRotateXYZ(camRotation, m_context.camPitch, m_context.camYaw, 0.0f);

    float camTranslation[16];
    bx::mtxTranslate(camTranslation, 0.0f, 0.0f, -5.0f);

    float camTransform[16];
    bx::mtxMul(camTransform, camTranslation, camRotation);

    float view[16];
    bx::mtxInverse(view, camTransform);

    float proj[16];
    bx::mtxProj(
        proj, 60.0f, float(m_context.width) / float(m_context.height), 0.1f,
        100.0f, bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

    float model[16];
    bx::mtxIdentity(model);
    bgfx::setTransform(model);

    bgfx::setVertexBuffer(0, m_context.vertexBufferHandle);
    bgfx::setIndexBuffer(m_context.indexBufferHandle);

    bgfx::submit(0, m_context.programHandle);

    bgfx::frame();
}

void Renderer::cleanup() {
    bgfx::destroy(m_context.vertexBufferHandle);
    bgfx::destroy(m_context.indexBufferHandle);
    bgfx::destroy(m_context.programHandle);

    bgfx::shutdown();

    SDL_DestroyWindow(p_window);
    SDL_Quit();
    
}

bool Renderer::getActive() {
    return m_active;
}
