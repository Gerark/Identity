cmake_minimum_required(VERSION 3.0)

# Set the project name and version
project(MyProject VERSION 1.0)

# Add the executable
add_executable(MyProject 
    src/main.cpp
    external/imgui/imgui_impl_sdl2.cpp
    external/imgui/imgui_impl_opengl3.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp)

# Check if Emscripten is being used
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1")
endif()