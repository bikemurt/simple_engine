#ifndef GUI_H
#define GUI_H

// MODULES
#include "SDL2/SDL.h"
#include "bx/bx.h"

namespace SimpleEngine {

class GUI {
public:
    GUI();
    void setup(SDL_Window* p_window);
    void cleanup();
    void update();
};

}

#endif