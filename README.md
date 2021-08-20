# GravitySimulator
A 2D simulation of newtonian gravity. It is meant to help physics students understand gravity from a more "hands-on" perspective.

# Dependencies

This project depends on [SDL2](https://www.libsdl.org/index.php), [Dear ImGui](https://github.com/ocornut/imgui), and [GLAD](https://glad.dav1d.de/) (OpenGL 3.3 is required at a minimum). Both Dear ImGui and GLAD are included in the repository; however, SDL2 must be downloaded separately.

# Releases

Binaries will be provided for Windows 10 only. They come with an SDL2 dll, so no external dependencies are required.

# How to compile

Currently, this project must be compiled through Visual Studio, although that will change in the next version.

To compile, first [download](https://www.libsdl.org/download-2.0.php) the newest version of SDL2's Visual Studio development library. The file should look like `SDL2-devel-___-VC.zip`. Unzip this file into the `GravitySimulator\SDL2\` directory.

Then, install Visual Studio with minimal support for C++ (only the MSVC compiler is required and the standard library). Open the sln file in Visual Studio, and compile for your target architecture (e.g., Release x64). Once the executable is compiled, drag the SDL.dll file from the previous zip file into the executable's directory.

# Hopeful future additions

* Linux support
* Checks for if SDL and/or OpenGL fails
* A better commented codebase
