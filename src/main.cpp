#include "fmt/format.h"

#include "core/renderer.h"
#include "core/gui.h"

using namespace SimpleEngine;

int main(int argc, char **argv) {

	GUI gui;

	// GUI's setup/update/cleanup calls are embedded into renderer
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