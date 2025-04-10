# Define project
cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
project(ZephyrGL LANGUAGES CXX VERSION 0.1.0)

# Disable console on Windows
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()


# Optimisations depending build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    message(STATUS "Building en Debug mode")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-Wall -pedantic-errors -Wextra -O0)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        add_compile_options(/W4)
    endif()
else()
    add_compile_definitions(NDEBUG)
    message(STATUS "Building en Release mode")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-O2)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        add_compile_options(/O2)
    endif()
endif()

# Find required packages
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Variables
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "lib/*.cpp")
set(LIBS glfw GLEW::GLEW)

# Create executable
add_executable(ZephyrGL ${SOURCES})

# Link libraries, depending on platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND LIBS mingw32)
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND LIBS -fsanitize=address -fno-omit-frame-pointer)
endif()

target_link_libraries(ZephyrGL ${LIBS})

# Test build
option(BUILD_TESTING "Enable testing" OFF)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory("tests")
endif()