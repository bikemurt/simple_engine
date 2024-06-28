# Simple Engine

A Simple Game Engine (3D) with a focus on integrating high quality, high visibility, open-source C++ libraries.

Right now it's just authored by me, but I am looking for collaborators - please [reach out](https://x.com/_michaeljared) if you are interested.

## Goals:

- Build a game engine in C++, using lightweight tools such as `vcpkg`, `cmake` and `Visual Studio Code`
- Keep code organized, lightweight, minimal
- An editor that is built from the engine itself

## Third party libraries (links are to `vcpkg`, check dependencies there):
- 3D renderer - [bgfx](https://vcpkg.io/en/package/bgfx) `in process`
- Model loading - [tinyGLTF](https://vcpkg.io/en/package/tinygltf) `in process`
- Windowing and input system - [SDL2.0](https://vcpkg.io/en/package/sdl2.html) `in process`
- Physics collisions - [JoltPhysics](https://vcpkg.io/en/package/joltphysics) `not started`
- Audio system - [OpenALSoft](https://vcpkg.io/en/package/openal-soft) `not started`
- 2D UI elements - [ImGUI](https://vcpkg.io/en/package/imgui) `not started`
- Networking -[SDL2-net](https://vcpkg.io/en/package/sdl2-net) `not started`

## Project principles:
- A focus on pedagogy. Anyone forking this should be able to build it, learn from it quickly, and work on it if they want 
- Minimal dependencies in third party libraries. You can check each of the above libraries in `vcpkg`

## Other things that may or may not be important
- Engine wrapper classes are encouraged to make code style consistent, even if they are effectively pass-through functions to third-party libraries
- Target C++17
- Only manage memory where needed - allow libraries to do it when they should do it
- Avoid singletons (if possible, entirely) for the purposes of unit testing

## Timeline:
None. This is for fun/learning.
