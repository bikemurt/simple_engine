#ifndef RENDERER_H
#define RENDERER_H


#include "bx/bx.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "GLFW/glfw3.h"

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include "GLFW/glfw3native.h"

class Renderer {
private:
    GLFWwindow* p_window;
    int displayWidth;
    int displayHeight;
    int windowWidth;
    int windowHeight;

    const bgfx::ViewId kClearView = 0;
    static bool showStats;

    static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

public:
    void setup();
    void renderFrame();
    void cleanup();

    GLFWwindow* getWindow();
};

#endif