cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME MyProject)
set(COMMON_SOURCES src/main.cpp)

# Set the project name and version
project(${TARGET_NAME}  VERSION 1.0)

# Add the executable
add_executable(${TARGET_NAME} src/main.cpp)

# Check if Emscripten is being used
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 --shell-file ../src/emscripten_shell.html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2")
    target_sources(${TARGET_NAME} PRIVATE
        external/imgui/imgui/backends/imgui_impl_sdl2.cpp
        external/imgui/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/imgui/imgui.cpp
        external/imgui/imgui/imgui_draw.cpp
        external/imgui/imgui/imgui_tables.cpp
        external/imgui/imgui/imgui_widgets.cpp)
    # Include the required directories
    include_directories(external/imgui external/imgui/imgui)
else()
    # Setup SDL2 package
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/external/sdl2/cmake")
    find_package(SDL2 REQUIRED)
    # Create a wrapper library for SDL2
    add_library(sdl2_wrapper INTERFACE)
    # Link the wrapper library with SDL2 and SDL2main
    target_link_libraries(sdl2_wrapper INTERFACE SDL2::SDL2 SDL2::SDL2main)

    find_package(OpenGL REQUIRED)

    # Include the required directories
    include_directories(${OPENGL_INCLUDE_DIRS})

    # Adding glad as Loader-Generator for opengl
    add_subdirectory(external/glad)
    # Adding imgui
    add_subdirectory(external/imgui)

    # Link the required libraries
    target_link_libraries(${TARGET_NAME} sdl2_wrapper)
    target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARIES})
    target_link_libraries(${TARGET_NAME} glad)
    target_link_libraries(${TARGET_NAME} imgui)

    # Copy all the dependencies ( dll ) to the executable file directory
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${TARGET_NAME}> $<TARGET_RUNTIME_DLLS:${TARGET_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()