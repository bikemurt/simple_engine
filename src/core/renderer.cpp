#include "renderer.h"

#include "fmt/format.h"

bool Renderer::showStats = false;

GLFWwindow* Renderer::getWindow() {
    return p_window;
}

void Renderer::glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE) {
		showStats = !showStats;
	}
}

void Renderer::setup()
{

    glfwSetErrorCallback([](int error, const char* description) {
		fmt::println(stderr, "Glfw error {}: {}", error, description);
    });

	if (!glfwInit()) return;

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window with graphics context

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	p_window = glfwCreateWindow(800, 600, "My GLFW Window", nullptr, nullptr);
	if (!p_window) return;
	
	glfwSetKeyCallback(p_window, glfw_keyCallback);

	bgfx::renderFrame();

	bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(p_window);
#elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(p_window);
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(p_window);
#endif
	glfwGetWindowSize(p_window, &windowWidth, &windowHeight);
	init.resolution.width = (uint32_t)windowWidth;
	init.resolution.height = (uint32_t)windowHeight;
	init.resolution.reset = BGFX_RESET_VSYNC;
	if (!bgfx::init(init)) {
		return;
	}

	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Renderer::renderFrame() {

    glfwPollEvents();

	int oldWidth = windowWidth;
	int oldHeight = windowHeight;
	glfwGetWindowSize(p_window, &windowWidth, &windowHeight);
	
	if (windowWidth != oldWidth || windowHeight != oldHeight) {
		bgfx::reset((uint32_t)windowWidth, (uint32_t)windowHeight, BGFX_RESET_VSYNC);
		bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
	}

	// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
	bgfx::touch(kClearView);
	// Use debug font to print information about this example.
	bgfx::dbgTextClear();
	
	bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
	bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
	bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
	bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
	const bgfx::Stats* stats = bgfx::getStats();
	bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
	
	// Enable stats or debug text.
	bgfx::setDebug(showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
	
	// Advance to next frame. Process submitted rendering primitives.
	bgfx::frame();
}

void Renderer::cleanup() {
	bgfx::shutdown();

	glfwDestroyWindow(p_window);
	glfwTerminate();
}