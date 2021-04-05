# ========================================================
# QOMBAT BUILD SYSTEM
# This is the CMakeLists.txt that generates the Static
# Library for the Engine, which will then be linked to the
# to the frontend Application. 
# ========================================================
project(Engine)
# We add all the source files manually to the library instead of 
# GLOB/Wildcards. This is recommended by the CMake documentation.
add_library(Engine STATIC 
"Source/QMBT.hpp"
"Source/Core/Application.cpp"
"Source/Core/Log.cpp"
"Source/Core/Console.cpp"
"Source/Platform/OS/Linux/LinuxWindow.cpp"
"Source/Core/Memory/PoolAllocator.cpp"
"Source/Core/Memory/StackAllocator.cpp"
"Source/Core/Memory/MemoryManager.cpp"

)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/QMBTPCH.hpp")

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
      _CRT_SECURE_NO_WARNINGS
    PUBLIC
      $<$<CONFIG:Debug>:QMBT_DEBUG>
      $<$<CONFIG:RelWithDebInfo>:QMBT_DEBUG>
      $<$<CONFIG:Release>:QMBT_RELEASE>
      $<$<CONFIG:MinSizeRel>:QMBT_RELEASE>
)


target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# =======================================================
# Appending to CMAKE_PREFIX_PATH for vendor libraries
# =======================================================


# =======================================================
# Searching for Vendor Packages
# =======================================================

# =======================================================
# Including vendor libraries
# =======================================================
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Vendor")

# =======================================================
# Adding libraries with CMakeLists.txt as subdirectories
# =======================================================
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/magic_enum")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw")

# =======================================================
# Linking vendor libraries
# =======================================================
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE magic_enum::magic_enum)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)



