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

#include <string>

class Renderer {
private:

    SDL_Window* p_window;

    struct Context {
        uint32_t width = 800;
        uint32_t height = 600;

        // library types in an internal class are OK
        bgfx::VertexLayout layout;
        bgfx::VertexBufferHandle vertexBufferHandle;
        bgfx::IndexBufferHandle indexBufferHandle;
        bgfx::ProgramHandle programHandle;

        float camPitch = 0.0f;
        float camYaw = 0.0f;
        float rotScale = 0.01f;

        int prevMouseX = 0;
        int prevMouseY = 0;

    } m_context;

    void handleEvents();
    bgfx::ShaderHandle createShader(const std::string& shader, const char* name);

public:

    bool m_active = true;

    Renderer();

    void setup();
    void renderFrame();
    void cleanup();

    struct PosColorVertex
    {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_abgr;
    };

};

#endif