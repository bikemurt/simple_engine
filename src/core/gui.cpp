// CORE
#include "gui.h"
#include "renderer.h"
#include "../importers/gltf_loader.h"
#include "../utils/fileops.h"

// MODULES
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "../external/bgfx_imgui/imgui_impl_bgfx.h"

#include "fmt/format.h"

using SimpleEngine::GUI;
using namespace SimpleEngine::External;

GUI::GUI() {

}

void GUI::setup() {
    assert(p_renderer != nullptr);

    ImGui::CreateContext();
    ImGui_Implbgfx_Init(255);

    // I don't really have full support for emscripten yet, see TODO in Renderer
#if BX_PLATFORM_WINDOWS
    ImGui_ImplSDL2_InitForD3D(p_renderer->p_window);
#elif BX_PLATFORM_OSX
    ImGui_ImplSDL2_InitForMetal(p_window);
#elif BX_PLATFORM_LINUX || BX_PLATFORM_EMSCRIPTEN
    ImGui_ImplSDL2_InitForOpenGL(p_window, nullptr);
#endif

    NFD_Init();

}

void GUI::fileDialog(std::string& outPathStr, nfdresult_t& result) {
    nfdu8char_t* outPath;
    nfdu8filteritem_t filters[1] = { { "GLTF File", "gltf,glb" } };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    result = NFD_OpenDialogU8_With(&outPath, &args);
    if (result == NFD_OKAY)
    {
        fmt::println("Success!");
        fmt::println(outPath);
        
        outPathStr = std::string(outPath);

        NFD_FreePathU8(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        fmt::println("User pressed cancel.");
    }
    else 
    {
        fmt::println("Error: %s\n", NFD_GetError());
    }
}

void GUI::update() {
    
    ImGui_Implbgfx_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    
    ImGui::Text(fileStatus.c_str());
    if (ImGui::Button("Import a Model")) {
        nfdresult_t result;
        std::string path;
        fileDialog(path, result);

        std::string ext = FileOps::getFilePathExtension(path);
        if (result == NFD_OKAY && (ext.compare("gltf") == 0 || ext.compare("glb") == 0)) {
            GltfLoader g(path, p_renderer);
            p_renderer->postImportAddToSceneTree();
            fileStatus = "Model loaded: " + path;
        } else {
            fileStatus = "Invalid file: " + path;
        }
    }
    
    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
}

void GUI::cleanup() {

    ImGui_ImplSDL2_Shutdown();
    ImGui_Implbgfx_Shutdown();

    ImGui::DestroyContext();

    NFD_Quit();
}