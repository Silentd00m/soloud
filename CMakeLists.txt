cmake_minimum_required (VERSION 2.6)
project(SoLoud)

file(GLOB_RECURSE soloud_SOURCES src/audiosource/* src/filter/* src/core/*)

if(WIN32)
    find_library(soloud_PLATFORM_LIBRARIES Winmm)
    file(GLOB_RECURSE soloud_PLATFORM_SOURCES src/backend/winmm/*)
    set(soloud_PLATFORM_DEFINES "WITH_WINMM")
else()
    message(FATAL_ERROR "Could not detect platform!")
endif()

add_library(soloud ${soloud_SOURCES} ${soloud_PLATFORM_SOURCES})
target_include_directories(soloud PUBLIC include)
set_property(TARGET soloud APPEND PROPERTY COMPILE_DEFINITIONS
  ${soloud_PLATFORM_DEFINES})
target_link_libraries(soloud ${soloud_PLATFORM_LIBRARIES})
