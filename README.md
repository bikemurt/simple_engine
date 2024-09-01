# SimpleEngine

A simple 3D game engine with a focus on integrating high quality, high visibility, open-source C++ libraries.

![image](https://github.com/bikemurt/simple_engine/assets/23486102/1d12a829-1043-4484-9a29-d6e47774470e)

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
- 2D UI elements - [ImGUI](https://vcpkg.io/en/package/imgui) `lib downloaded`
- Networking - [SDL2-net](https://vcpkg.io/en/package/sdl2-net) `not started`

## Project principles:
- A focus on pedagogy. Anyone forking this should be able to build it, learn from it quickly, and work on it if they want 
- Minimal dependencies in third party libraries. You can check each of the above libraries in `vcpkg`
- Quick startup for building and running from source

## Other things that may or may not be important:
- Target C++17
- Only manage memory you absolutely have to. Allow libs to manage their own memory
- Avoid singletons (if possible, entirely) for the purposes of unit testing

## Timeline:
None. This is for fun/learning.

## Building from Source:
Clone the repo (fork and clone if you want to contribute). It is expected that `CMake` and `Visual Studio Code` will be used, so you'll need to make sure you have all the tooling installed properly.

If you aren't familiar with `vcpkg`, take a look at `vcpkg.json` above. It's fairly intuitive. It contains all dependencies and tools/features required.

In `vscode` I'm using the `CMake Tools` extension, and there's one additional setting needed in order to integrate with `vcpkg`:
![image](https://github.com/bikemurt/simple_engine/assets/23486102/49cea7ce-8140-4d19-8a9d-ca4b4ff6d033)

It incorporates the `vcpkg.cmake` file into the toolchain (you'll need to set up the [vcpkg/CMake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vscode?pivots=shell-cmd#1---set-up-vcpkg) tooling first):

`-DCMAKE_TOOLCHAIN_FILE=C:\path_to_vcpkg\scripts\buildsystems\vcpkg.cmake`

Hit `Ctrl+Shift+P` to bring up the command pallette in `VSCode`. Scan for toolkits in CMake, select the appropriate one, then configure the project. Afterwards you should be able to build using the cmake build command. First time build might take a few minutes to download all the dependencies and build, prior to building this application.

## First time running SimpleEngine:

It will probably crash immediately. This is likely because the shaders aren't built.

1. Navigate to `assets/shaders` in a terminal
2. Run `.\compile_shaders_win.bat`

Try re-running SimpleEngine.
