cmake_minimum_required (VERSION 2.8.4)
project(SoLoud)

file(GLOB_RECURSE soloud_SOURCES src/audiosource/* src/filter/* src/core/*)

set(soloud_PLATFORM_LIBRARIES "" CACHE FILEPATH "Path to your platform's sound library.")
set(soloud_PLATFORM_LIBRARY_DEFINE "SDL2_STATIC" CACHE STRING "Name of the platform to use.")

add_library(soloud STATIC ${soloud_SOURCES} ${soloud_PLATFORM_SOURCES})
target_include_directories(soloud PUBLIC include)
target_compile_definitions(soloud PUBLIC "-DWITH_${soloud_PLATFORM_LIBRARY_DEFINE}")
