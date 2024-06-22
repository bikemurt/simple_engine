#include "fmt/format.h"

#include "core/model_loader.h"
#include "core/renderer.h"

static void hello_world() {
	ImGui::Begin("My DearImGui Window");
	ImGui::Text("hello, world");
	ImGui::End();
}

int main() {

	// stack allocated for now
	// need to get smarter with heap allocations inside classes
	Renderer renderer;
	renderer.initialize();
	
	ModelLoader modelLoader;
	modelLoader.loadModel();

	fmt::println("loop starting");

	while (!glfwWindowShouldClose(renderer.getWindow())) {

		renderer.pollEvents();
		renderer.startGUI();

		// Run our Dear ImGui application
		hello_world();

		// Rendering
		renderer.renderInit();
	}

	renderer.cleanup();
}