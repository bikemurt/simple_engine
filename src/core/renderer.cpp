#include "renderer.h"

#include "fmt/format.h"

Renderer::Renderer() : active(true) {

}

void Renderer::setup() {
	SDL_Init(SDL_INIT_VIDEO);
	
    width = 800;
    height = 600;

    // TODO: platform specific stuff here
    p_window = SDL_CreateWindow(
        "bgfx-SDL2 test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(p_window, &wmi);

    bgfx::renderFrame();

    bgfx::PlatformData platformData;
    platformData.ndt = nullptr;
    platformData.nwh = wmi.info.win.window;

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = platformData;

    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);

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

void Renderer::renderFrame() {

    handleEvents();
    
    bgfx::touch(0);
    bgfx::frame();
}

void Renderer::cleanup() {
    bgfx::shutdown();
    SDL_Quit();
}

bool Renderer::isActive() {
    return active;
}
