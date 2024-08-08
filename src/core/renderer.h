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
#include <fstream>
#include <filesystem>

#include "tiny_gltf.h"

class Renderer {
public:

    // Renderer::Context should also exist in Mesh
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

    };

private:

    SDL_Window* p_window;
    SDL_SysWMinfo wmi;

    Context context;


    void setupWindow();
    void handleEvents();
    void cleanupWindow();
    
    std::string meshImportSavePath = "C:\\Projects\\CppTesting\\simple_engine\\imports\\meshes\\";
    struct RenderObject {
        // shouldn't carry this stuff around, but oh well
        std::string specifier = "";

        std::vector<uint8_t> vertexData;
        std::vector<uint16_t> indexData;
        bgfx::VertexBufferHandle vertexBufferHandle;
        bgfx::IndexBufferHandle indexBufferHandle;
    };

    std::vector<RenderObject> renderObjects;
    
    void saveRenderObject(const RenderObject& renderObject);
    void loadRenderObject(const std::string& specifier, RenderObject* renderObject);
    void processPrimitive(const tinygltf::Model &model, const tinygltf::Primitive &primitive, RenderObject *renderObject);
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

    bool active = true;

    Renderer();

    void setup();
    void renderFrame();
    void cleanup();
};

#endif