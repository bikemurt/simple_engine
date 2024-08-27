#include "fmt/format.h"

#include "core/renderer.h"

using namespace SimpleEngine;

int main(int argc, char **argv) {

	Renderer renderer;
	renderer.setup();
	
	fmt::println("loop starting");

	while (renderer.active) {

		renderer.renderFrame();
	}

	fmt::println("shutdown");
	renderer.cleanup();

	return 0;
}