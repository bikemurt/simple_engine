#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"
#include "bx/math.h"

#include <string>
#include <vector>

#include "tiny_gltf.h"

class Renderer {
private:

    SDL_Window* p_window;
    SDL_SysWMinfo wmi;

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


    void setupWindow();
    void handleEvents();
    void cleanupWindow();
    
    
    std::vector<uint8_t> vData;
    std::vector<uint16_t> iData;

    void loadModel(const std::string& myFile);

    struct GLTFBufferData {
        size_t count = 0;
        int type = -1;
        int byteStride = 0;
        int buffer = -1;
        size_t offset = 0;
        size_t length = 0;
    };
    void findDataFromAttribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData* bufferData);
    void get8BitFrom16BitUInt(uint8_t* dest, const void* source);

    bgfx::ShaderHandle createShader(const std::string& shader, const char* name);

public:

    bool m_active = true;

    Renderer();

    void setup();
    void renderFrame();
    void cleanup();
};

#endif