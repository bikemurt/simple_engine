#ifndef GUI_H
#define GUI_H

// CORE
#include "node.h"

// MODULES
#include "SDL2/SDL.h"
#include "bx/bx.h"
#include "nfd.h"

// STDLIB
#include <string>
#include <memory>

namespace SimpleEngine {

class Renderer;

class GUI {

friend class Renderer;

private:
    void fileDialog(std::string& outPathStr, nfdresult_t& result);
    std::string fileStatus = "(file status)";

    // this gets set in Renderer's constructor
    Renderer* p_renderer;
    uint16_t lastSceneTreeChangedCounter = 65535;

    void processSceneTree(const std::unique_ptr<Node>& node);
    void generateSceneTree();

public:
    GUI();

    void setup();
    void cleanup();
    void update();
};

}

#endif