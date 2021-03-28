# ========================================================
# QOMBAT BUILD SYSTEM
# This is the CMakeLists.txt that generates the Static
# Library for the Engine, which will then be linked to the
# to the frontend Application. 
# ========================================================

# We add all the source files manually to the library instead of 
# GLOB/Wildcards. This is recommended by the CMake documentation.
add_library(Engine STATIC 
"Source/QMBT.hpp"
"Source/Core/Application.cpp"
"Source/Core/Log.cpp"
"Source/Core/Console.cpp"

)

set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_precompile_headers(Engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/QMBTPCH.hpp")

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# =======================================================
# Including vendor libraries
# =======================================================
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/Vendor")

# =======================================================
# Adding libraries with CMakeLists.txt as subdirectories
# =======================================================
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog")


# =======================================================
# Linking vendor libraries
# =======================================================
target_link_libraries(Engine PUBLIC spdlog::spdlog)
