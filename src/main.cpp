#include "fmt/format.h"

#include "core/renderer.h"
#include "core/gui.h"

using namespace SimpleEngine;

int main(int argc, char **argv) {

	// big modules live in main for memory
	GUI gui;

	// the plan is to do dependency injection for most things into the renderer
	Renderer renderer(gui);
	renderer.setup();
	
	fmt::println("loop starting");

	while (renderer.active) {
		renderer.update();
	}

	fmt::println("shutdown");
	renderer.cleanup();

	return 0;
}