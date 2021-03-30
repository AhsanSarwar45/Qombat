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
"Source/Platform/OS/Linux/LinuxWindow.cpp"
)

set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_precompile_headers(Engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/QMBTPCH.hpp")

target_compile_definitions(Engine
    PRIVATE
      _CRT_SECURE_NO_WARNINGS
    PUBLIC
      $<$<CONFIG:Debug>:QMBT_DEBUG>
      $<$<CONFIG:RelWithDebInfo>:QMBT_DEBUG>
      $<$<CONFIG:Release>:QMBT_RELEASE>
      $<$<CONFIG:MinSizeRel>:QMBT_RELEASE>
)


target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# =======================================================
# Appending to CMAKE_PREFIX_PATH for vendor libraries
# =======================================================


# =======================================================
# Searching for Vendor Packages
# =======================================================

# =======================================================
# Including vendor libraries
# =======================================================
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/Vendor")

# =======================================================
# Adding libraries with CMakeLists.txt as subdirectories
# =======================================================
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/magic_enum")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw")

# =======================================================
# Linking vendor libraries
# =======================================================
target_link_libraries(Engine PUBLIC spdlog::spdlog)
target_link_libraries(Engine PRIVATE magic_enum::magic_enum)
target_link_libraries(Engine PRIVATE glfw)



