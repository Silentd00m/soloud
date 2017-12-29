cmake_minimum_required (VERSION 2.8.4)
project(SoLoud)

file(GLOB_RECURSE soloud_SOURCES src/audiosource/*.cpp src/filter/*.cpp
  src/core/*.cpp)

set(soloud_PLATFORM_LIBRARIES "" CACHE FILEPATH "Path to your platform's sound library.")
set(soloud_PLATFORM_LIBRARY_DEFINE "SDL2_STATIC" CACHE STRING "Name of the platform to use.")

if(soloud_PLATFORM_LIBRARY_DEFINE STREQUAL "SDL2_STATIC")
  file(GLOB soloud_BACKEND_SOURCES src/backend/sdl2_static/*.cpp)
endif()

add_library(soloud_obj OBJECT ${soloud_SOURCES} ${soloud_BACKEND_SOURCES})
target_include_directories(soloud_obj PUBLIC include)
target_compile_definitions(soloud_obj PUBLIC "-DWITH_${soloud_PLATFORM_LIBRARY_DEFINE}")
set_property(TARGET soloud_obj PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(soloud_shared SHARED $<TARGET_OBJECTS:soloud_obj>)
add_library(soloud_static STATIC $<TARGET_OBJECTS:soloud_obj>)

target_link_libraries(soloud_shared ${SDL2})

if(NOT WIN32)
  set_target_properties(soloud_shared PROPERTIES OUTPUT_NAME soloud)
  set_target_properties(soloud_static PROPERTIES OUTPUT_NAME soloud)
endif()
