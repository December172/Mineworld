cmake_minimum_required(VERSION 3.10..3.30)

project(Mineworld VERSION 0.0.1
                  DESCRIPTION "Minicraft clone in C++"
                  LANGUAGES CXX)

if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please create a build directory outside of the source directory.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_SOURCE_CODE_DIR ${PROJECT_SOURCE_DIR}/src)
set(PROJECT_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(PROJECT_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/libs)

# External dependencies list (header only libs were not included in DEPENDENCIES)
# Lua: version 5.4.7, -std=c11, -Os
# Libzip: version 1.10.1, without lzma or bzip2 support
# GLM: version 1.0.1, header only
include_directories(${PROJECT_EXTERNAL_DIR})
set(DEPENDENCIES mingw32 SDL2main SDL2 opengl32 glew32 zip z)

#add_subdirectory(${PROJECT_SOURCE_CODE_DIR})
include_directories(${PROJECT_SOURCE_CODE_DIR})
link_directories(${PROJECT_LIBRARY_DIR})
link_directories(${PROJECT_LIBRARY_DIR}/SDL2)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_CODE_DIR}/*.cpp")

# Overall compiler options
add_compile_options("-std=c++17")
# Warnings
add_compile_options("-Wall" "-Wextra")
# Defines
add_compile_definitions("PLOG_CHAR_IS_UTF8" "MINEWORLD_FPS_COUNTER")
# Debug options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options("-O0" "-g")
endif()
# Release options
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options("-Ofast" "-Os" "-s" "-fno-fast-math" "-fno-signed-zeros" "-ffinite-math-only" "-frename-registers" "-flto")
    add_link_options("-s")
endif()


add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})