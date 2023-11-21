# CMakeList.txt : CMake project for CompArchProj1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CompArchProj1")

# Add source to this project's executable.
add_executable (CompArchProj1 "CompArchProj1.cpp" "CompArchProj1.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CompArchProj1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
