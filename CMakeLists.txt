cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(TARGET_NAME Identity)
set(COMMON_SOURCES src/main.cpp src/LittleEngine.cpp src/LittleEngine.h src/EngineImpl.cpp src/EngineImpl.h)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check if Emscripten is being used
if(EMSCRIPTEN)    
    # Set the project name and version
    project(${TARGET_NAME} VERSION 1.0)

    # Add the executable
    add_executable(${TARGET_NAME} ${COMMON_SOURCES})

    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s USE_SDL=2 -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 -std=c++2b")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2")

    # Let's manually add the imgui sources to simplify the build process
    target_sources(${TARGET_NAME} PRIVATE
        external/imgui/imgui/backends/imgui_impl_sdl2.cpp
        external/imgui/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/imgui/imgui.cpp
        external/imgui/imgui/imgui_draw.cpp
        external/imgui/imgui/imgui_tables.cpp
        external/imgui/imgui/imgui_widgets.cpp
        external/stb/stb/stb_image.h)
    
    # Include the required directories
    include_directories(external/imgui external/imgui/imgui external/stb)    
else()
    # Set C++23 as the standard to be used
    set(CMAKE_CXX_STANDARD 23)
    # Generate an error if C++23 support is not available
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Avoid conflicts with <Windows.h> min-max functions
    add_compile_definitions(NOMINMAX)

    # Suppress some compiler warnings to reduce noise
    add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

    # Treat warnings as errors
    add_compile_options(/WX)
    # Set warning level 4
    add_compile_options(/W4)
    
    # Set the project name and version
    project(${TARGET_NAME} VERSION 1.0)

    # Add the executable
    add_executable(${TARGET_NAME} ${COMMON_SOURCES})

    # Include macros from the cmakes folder
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakes)
    include(folder_utils)

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
    # Adding stb_image
    add_subdirectory(external/stb)

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

    # Copy the data folder to the executable file directory
    add_custom_target(RunPostBuildCommand ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${TARGET_NAME}>/data
        COMMAND_EXPAND_LISTS
    )

    # Set the working directory for visual studio
    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET_NAME}>
    )
    
    add_to_apps_folder(${TARGET_NAME} "${COMMON_SOURCES}")
    
    # Set the project as the default startup project for the generated Visual Studio solution
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
    
    set_property(TARGET glad imgui PROPERTY FOLDER "external")
    # Set a custom folder for the predefined targets in the generated Visual Studio solution
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmakeAndPostBuild")
endif()