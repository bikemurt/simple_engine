# Simple Engine

A Simple Game Engine (3D) with a focus on integrating high quality, high visibility, open-source C++ libraries.

Right now it's just authored by me, but I am looking for collaborators - please [reach out](https://x.com/_michaeljared) if you are interested.

## Goals:

- Build a game engine in C++, using lightweight tools such as `vcpkg`, `cmake` and `Visual Studio Code`
- Keep code organized, lightweight, minimal
- An editor that is built from the engine itself

_Note: right now it is NOT cross-platform (I am developing on Windows), but I intend to develop out the macros and code to facilitate that (eventually)._

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

## Other things that may or may not be important:
- Engine wrapper classes are encouraged to make code style consistent, even if they are effectively pass-through functions to third-party libraries
- Target C++17
- Only manage memory where needed - allow libraries to do it when they should do it
- Avoid singletons (if possible, entirely) for the purposes of unit testing

## Timeline:
None. This is for fun/learning.

## Building from Source:
Clone the repo (fork and clone if you want to contribute). It is expected that `CMake` and `Visual Studio Code` will be used.

If you aren't familiar with `vcpkg`, take a look at `vcpkg.json` above. It's fairly intuitive. It contains all dependencies and tools/features required.

In `vscode` I'm using the `CMake Tools` extension, and there's one additional setting needed in order to integrate with `vcpkg`:
![image](https://github.com/bikemurt/simple_engine/assets/23486102/49cea7ce-8140-4d19-8a9d-ca4b4ff6d033)

It incorporates the `vcpkg.cmake` file into the toolchain:

`-DCMAKE_TOOLCHAIN_FILE=C:\Projects\CppTesting\vc_package_test\vcpkg\scripts\buildsystems\vcpkg.cmake`

Hit `Ctrl+Shift+P` to bring up the command pallette in `VSCode`. Scan for toolkits in CMake, select the appropriate one, then configure the project. Afterwards you should be able to build using the cmake build command.
