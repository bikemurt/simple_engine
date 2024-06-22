#ifndef RENDERER_H
#define RENDERER_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Renderer {
private:
    GLFWwindow* p_window;
    int displayWidth;
    int displayHeight;

public:
    void initialize();
    void pollEvents();
    void startGUI();
    void renderInit();
    void cleanup();

    GLFWwindow* getWindow();
    int getDisplayWidth();
    int getDisplayHeight();
};

#endif