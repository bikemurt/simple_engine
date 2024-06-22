#include "fmt/format.h"

#include "core/model_loader.h"
#include "core/renderer.h"

int main() {

	// stack allocated for now
	// need to get smarter with heap allocations inside classes
	Renderer renderer;
	renderer.setup();
	
	ModelLoader modelLoader;
	modelLoader.loadModel();

	fmt::println("loop starting");

	while (!glfwWindowShouldClose(renderer.getWindow())) {

		renderer.renderFrame();
	}

	renderer.cleanup();
}