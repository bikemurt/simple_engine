// CORE
#include "gui.h"

// MODULES
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "../external/bgfx_imgui/imgui_impl_bgfx.h"

using SimpleEngine::GUI;
using namespace SimpleEngine::External;

GUI::GUI() {

}

void GUI::setup(SDL_Window* p_window) {
    ImGui::CreateContext();
    ImGui_Implbgfx_Init(255);

    // I don't really have full support for emscripten yet, see TODO in Renderer
#if BX_PLATFORM_WINDOWS
    ImGui_ImplSDL2_InitForD3D(p_window);
#elif BX_PLATFORM_OSX
    ImGui_ImplSDL2_InitForMetal(p_window);
#elif BX_PLATFORM_LINUX || BX_PLATFORM_EMSCRIPTEN
    ImGui_ImplSDL2_InitForOpenGL(p_window, nullptr);
#endif

}

void GUI::update() {
    
    ImGui_Implbgfx_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // your drawing here
    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

}

void GUI::cleanup() {

    ImGui_ImplSDL2_Shutdown();
    ImGui_Implbgfx_Shutdown();

    ImGui::DestroyContext();
}