#include "fmt/format.h"

#include "core/renderer.h"
#include "core/gui.h"

using namespace SimpleEngine;

int main(int argc, char **argv) {

	// big modules live in main for memory
	GUI gui;

	// GUI's setup/update/cleanup calls are embedded into renderer
	Renderer renderer(gui);
	
	// mutual coupling seems horrendous
	// better design pattern probably needed
	gui.p_renderer = &renderer;
	
	renderer.setup();
	
	fmt::println("loop starting");

	while (renderer.active) {
		renderer.update();
	}

	fmt::println("shutdown");
	renderer.cleanup();

	return 0;
}