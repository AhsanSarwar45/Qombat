# ===============================================
# QOMBAT BUILD SYSTEM

# This is the root CMakeLists.txt that set ups
# the general environment.
# ===============================================

cmake_minimum_required (VERSION 3.16) #3.16 for support for precompiled headers

project(Qombat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add sub directories
add_subdirectory(Editor)
add_subdirectory(Engine)