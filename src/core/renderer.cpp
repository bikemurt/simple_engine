#define ENABLE_DEBUG
#include "utils/debug.h"

#include "renderer.h"
#include "fmt/format.h"
#include "utils/fileops.h"

Renderer::Renderer()
    : m_active(true) {
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

void Renderer::setup() {

	SDL_Init(SDL_INIT_VIDEO);
	
    m_width = 800;
    m_height = 600;

    // TODO: platform specific stuff here
    p_window = SDL_CreateWindow(
        "bgfx-SDL2 test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_width, m_height, SDL_WINDOW_SHOWN);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(p_window, &wmi);

    bgfx::renderFrame();

    bgfx::PlatformData platformData;
    platformData.ndt = nullptr;
    platformData.nwh = wmi.info.win.window;

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = m_width;
    init.resolution.height = m_height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = platformData;

    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, m_width, m_height);

    // configure vertex layout in the GPU for indexed rendering of the triangles
    m_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    
    m_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        m_layout
        );

    m_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
        );

    const std::string shaderRoot = "../assets/shaders";
    
    std::string vShader;
    assert(FileOps::getFileContentsBinary("../../assets/shaders/vertex.sc", vShader));
    
    

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
    
    bgfx::touch(0);
    bgfx::frame();
}

void Renderer::cleanup() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
    bgfx::shutdown();
    SDL_Quit();
    
}

bool Renderer::getActive() {
    return m_active;
}
