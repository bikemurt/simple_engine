#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"
#include "bx/allocator.h"
#include "bx/file.h"

#include "bgfx/bgfx_shader.sh"

class Renderer {
private:

    SDL_Window* p_window;

    uint32_t m_width;
    uint32_t m_height;

    bool m_active;

    void handleEvents();

    bgfx::VertexLayout m_layout;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    

public:

    Renderer();

    void setup();
    void renderFrame();
    void cleanup();

    bool getActive();
    
    struct PosColorVertex
    {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_abgr;
    };


};

#endif