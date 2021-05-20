# QOMBAT

An RTS Game Engine based on the [Hazel](https://github.com/TheCherno/Hazel) Game Engine by [TheCherno](https://github.com/TheCherno)

## Links

- [Trello](https://trello.com/b/7q6IzuhJ/qombat)

## Dependencies

- [EASTL](https://github.com/electronicarts/EASTL)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Glad](https://github.com/Dav1dde/glad)
- [GLFW](https://github.com/glfw/glfw)
- [Magic Enum C++](https://github.com/Neargye/magic_enum)
- [spdlog](https://github.com/gabime/spdlog)
- [Catch2](https://github.com/catchorg/Catch2)

## Build Instructions

### Requirements

- Windows, Linux, MacOS
- CMake version 1.16+

### Steps

1.  Clone the git repository to local machine using: `git clone --recurse-submodules -j8 https://github.com/AhsanSarwar45/Qombat `
2.  You can now build either using the terminal or VSCode.

<!-- ### Using terminal

1. Navigate to cloned directory.
2. Type `mkdir build` to create the build directory.
3. Navigate to the directory using `cd build`.
4. Type `cmake ..\` to configure the cmake files.


```bash
   mkdir build
   cd build
   cmake ../Qombat
   cmake --build .
```

 -->

### Using the terminal

1. Navigate to the cloned directory.
2. Configure CMake using `cmake -B ../build -DCMAKE_BUILD_TYPE=[BuildType]`. Example for Debug build: `cmake -B ../build -DCMAKE_BUILD_TYPE=Debug`.
3. Build using `cmake --build ../build --config [BuildType] --target Editor`.

### Using VSCode + [CMake Tools extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

1.  Open cloned directory in VSCode.
2.  Press <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd> to open the command pallette.
3.  Type `CMake: Configure` to configure the CMake files.
4.  Select a compiler from the list.
5.  Type `CMake: Set Build Target` and select 'Editor'.
6.  Type `CMake: Build` to build the project.
