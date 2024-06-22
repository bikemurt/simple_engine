#include "locator.h"
#include "renderer.h"

Renderer* Locator::rendererService = nullptr;

Renderer* Locator::getRenderer() {
    return rendererService;
}

void Locator::provideRenderer(Renderer* service) {
    rendererService = service;
}