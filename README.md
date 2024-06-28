# Simple Engine

A Simple Game Engine with a focus on integrating high quality, high visibility, open-source C++ libraries.

Right now it's just authored by me, but I am looking for collaborators - please [reach out](https://x.com/_michaeljared) if you are interested.

## Goals:

- Build a game engine in C++, using lightweight tools such as `vcpkg`, `cmake` and `Visual Studio Code`
- Keep code organized, lightweight, minimal
- An editor that is built from the engine itself

Planned functions (all libs available in `vcpkg`):
- 3D renderer - [bgfx](https://github.com/bkaradzic/bgfx) `in process`
- Input system  - [SDL2.0](https://github.com/libsdl-org/SDL/tree/SDL2) `in process`
- Physics collisions - [JoltPhysics](https://github.com/jrouwe/JoltPhysics) `not started`
- Audio system - [OpenALSoft](https://github.com/kcat/openal-soft) `not started`
- 2D UI elements - [ImGUI](https://github.com/ocornut/imgui) `not started`
- Networking - Considering [CppServer](https://github.com/chronoxor/CppServer), however this is NOT available on vcpkg. Open to suggestions

You'll notice that each of the above libraries have 

## Design principles:
- Minimal dependencies in third party libraries
- Engine wrapper classes are encouraged to make code style consistent, even if they are effectively pass-through functions to third-party libraries
- Target C++17
- Only manage memory where needed - allow libraries to do it when they should do it
- Avoid singletons (if possible, entirely) for the purposes of unit testing

## Timeline:
None. This is for fun/learning.
