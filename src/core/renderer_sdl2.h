#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"

class Renderer {
private:

    SDL_Window* p_window;

    uint32_t width;
    uint32_t height;

    bool active;

public:

    Renderer();

    void setup();
    void renderFrame();
    void cleanup();

    bool isActive();
};

#endif