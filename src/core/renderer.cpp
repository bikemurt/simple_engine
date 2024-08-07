// core includes

#define ENABLE_DEBUG
#include "utils/debug.h"

#include "renderer.h"
#include "utils/fileops.h"

// module includes
#include "fmt/format.h"
#include <string>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

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

// --- TESTING

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

    loadModel("../../assets/gltf/cube.gltf");

    // configure vertex layout in the GPU for indexed rendering of the triangles
    context.layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    
    for (int i = 0; i < renderObjects.size(); i++) {
        auto r = renderObjects[i];

        renderObjects[i].vertexBufferHandle = bgfx::createVertexBuffer(
            bgfx::makeRef(&(renderObjects[i].vertexData[0]), renderObjects[i].vertexData.size() * sizeof(uint8_t)),
            context.layout
            );
        
        renderObjects[i].indexBufferHandle = bgfx::createIndexBuffer(
            bgfx::makeRef(&(renderObjects[i].indexData[0]), renderObjects[i].indexData.size() * sizeof(uint16_t))
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
    bgfx::destroy(context.vertexBufferHandle);
    bgfx::destroy(context.indexBufferHandle);
    bgfx::destroy(context.programHandle);

    bgfx::shutdown();

}

// --- MODEL LOADING

void Renderer::findDataFromAttribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const char* attribute, GLTFBufferData* bufferData) {
    std::map<std::string, int>::const_iterator pos = primitive.attributes.find(attribute);
    if (pos != primitive.attributes.end()) {
        
        int accessorIndex = pos->second;
        const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
        
        bufferData->count = accessor.count;
        bufferData->type = accessor.type;

        int bufferViewIndex = accessor.bufferView;
        const tinygltf::BufferView& bufferView = model.bufferViews[bufferViewIndex];
        
        bufferData->byteStride = accessor.ByteStride(bufferView);

        bufferData->buffer = bufferView.buffer;
        bufferData->offset = bufferView.byteOffset;
        bufferData->length = bufferView.byteLength;
    }
}

void Renderer::get8BitFrom16BitUInt(uint8_t* dest, const void* source) {
    uint16_t v_16;

    // a little bit obscure, but works
    // copy 2 bytes from source into v_16
    std::memcpy(&v_16, source, 2);

    // bit shift 8 bits to the right (lose the 8 LSB)
    // then assign to an 8 byte int (dest)
    *dest = v_16 >> 8;
}

// loadModel loads meshes from a gltf file into render objects
void Renderer::loadModel(const std::string& myFile) {

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string ext = FileOps::getFilePathExtension(myFile);
	
	bool ret = false;
	if (ext.compare("glb") == 0) {
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, myFile.c_str());
	} else {
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, myFile.c_str());
	}

    // this isn't great for obvious reasons (multiple gltf files loaded?)
    //renderObjects.resize(model.meshes.size());

    // for each mesh, we want a render object
	for (size_t i = 0; i < model.meshes.size(); i++) {
		const tinygltf::Mesh& mesh = model.meshes[i];

		for (size_t j = 0; j < mesh.primitives.size(); j++) {
			const tinygltf::Primitive& primitive = mesh.primitives[j];

			if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
                RenderObject o;
                
                GLTFBufferData posBufferData;
                findDataFromAttribute(model, primitive, "POSITION", &posBufferData);

                GLTFBufferData clrBufferData;
                findDataFromAttribute(model, primitive, "COLOR_0", &clrBufferData);

                assert(posBufferData.buffer != -1);
                assert(posBufferData.type == TINYGLTF_TYPE_VEC3);

                assert(clrBufferData.buffer != -1);
                assert(clrBufferData.type == TINYGLTF_TYPE_VEC4);

                int vertexCount = posBufferData.length / posBufferData.byteStride;

                // division by 2 to go from uint16_t to uint8_t
                o.vertexData.resize(posBufferData.length + clrBufferData.length / 2);

                const tinygltf::Buffer& clrBuffer = model.buffers[clrBufferData.buffer];
                const tinygltf::Buffer& posBuffer = model.buffers[posBufferData.buffer];

                // position data = 3 floats * 4 bytes per float = 12 bytes
                // R,G,B,A one byte each = 4 bytes
                int vertexByteStride = posBufferData.byteStride + clrBufferData.byteStride / 2;
                for (int i = 0; i < vertexCount; i++) {

                    // we can memcpy the 12 bytes of position data (3 floats, 4 bytes each)
                    std::memcpy(&o.vertexData[i * vertexByteStride + 0],
                        &posBuffer.data[posBufferData.offset + i * posBufferData.byteStride], posBufferData.byteStride);

                    // each color channel (RGBA) has to be converted from 16 bits to 8 bits
                    int clrOffset = clrBufferData.offset + i * clrBufferData.byteStride;

                    uint8_t a;
                    get8BitFrom16BitUInt(&a, &clrBuffer.data[clrOffset + 0]);
                    
                    uint8_t b;
                    get8BitFrom16BitUInt(&b, &clrBuffer.data[clrOffset + 2]);
                    
                    uint8_t g;
                    get8BitFrom16BitUInt(&g, &clrBuffer.data[clrOffset + 4]);
                    
                    uint8_t r;
                    get8BitFrom16BitUInt(&r, &clrBuffer.data[clrOffset + 6]);
                    
                    // manually packing in color data. structure is a-b-g-r
                    o.vertexData[i * vertexByteStride + 12] = a;
                    o.vertexData[i * vertexByteStride + 13] = b;
                    o.vertexData[i * vertexByteStride + 14] = g;
                    o.vertexData[i * vertexByteStride + 15] = r;
                }

                // index data is packed and can be memcpy'd
                const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];

                o.indexData.resize(indicesBufferView.byteLength / 2);
                std::memcpy(&o.indexData[0], &indicesBuffer.data[indicesBufferView.byteOffset], indicesBufferView.byteLength);
                
                renderObjects.push_back(o);
			}
		}

	}
}