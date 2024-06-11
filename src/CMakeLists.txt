# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    main.cpp
)

set(TARGET_NAME Identity)
add_executable(${TARGET_NAME} ${SOURCES})