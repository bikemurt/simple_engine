#include "renderer.h"

#include "fmt/format.h"

GLFWwindow* Renderer::getWindow() {
    return p_window;
}

int Renderer::getDisplayWidth() {
    return displayWidth;
}

int Renderer::getDisplayHeight() {
    return displayHeight;
}

void Renderer::initialize() {

    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Glfw error %d: %s\n", error, description);
    });

    glfwInit();

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window with graphics context

	p_window = glfwCreateWindow(800, 600, "My GLFW Window", nullptr, nullptr);
	glfwMakeContextCurrent(p_window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
	glewInit();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(p_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Main loop
	glfwGetFramebufferSize(p_window, &displayWidth, &displayHeight);

}

void Renderer::pollEvents() {
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();
}

void Renderer::startGUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::renderInit() {
    ImGui::Render();

    glfwGetFramebufferSize(p_window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(p_window);
}

void Renderer::cleanup() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(p_window);
	glfwTerminate();
}