cmake_minimum_required(VERSION 3.15)
project(NodeLayerDLL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC AND NOT CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
  message(WARNING "Recommended target for CANoe NodeLayer DLL is Win32 (-A Win32).")
endif()

if(MINGW)
  add_compile_options(-m32)
  add_link_options(-m32 -static-libgcc -static-libstdc++ -Wl,--kill-at)
endif()

add_library(NodeLayerDLL SHARED
  sources/NodeLayer.cpp
)

if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
  target_sources(NodeLayerDLL PRIVATE sources/NodeLayer.def)
endif()

set_target_properties(NodeLayerDLL PROPERTIES
  OUTPUT_NAME "NodeLayer"
  PREFIX ""
)

target_include_directories(NodeLayerDLL PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/includes
)
