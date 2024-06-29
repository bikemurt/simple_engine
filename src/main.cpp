#include "fmt/format.h"

#include "core/model_loader.h"

#include "core/renderer.h"

int main(int argc, char **argv) {

	Renderer renderer;
	renderer.setup();
	
	ModelLoader modelLoader;
	modelLoader.loadModel();

	fmt::println("loop starting");

	while (renderer.getActive()) {

		renderer.renderFrame();
	}

	fmt::println("shutdown");
	renderer.cleanup();

	return 0;
}