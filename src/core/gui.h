#ifndef GUI_H
#define GUI_H

// CORE

// MODULES
#include "SDL2/SDL.h"
#include "bx/bx.h"
#include "nfd.h"

// STDLIB
#include <string>

namespace SimpleEngine {

class Renderer;

class GUI {
private:
    void fileDialog(std::string& outPathStr, nfdresult_t& result);
    std::string fileStatus = "(file status)";

public:
    GUI();
    Renderer* p_renderer;

    void setup(SDL_Window* p_window);
    void cleanup();
    void update();
};

}

#endif