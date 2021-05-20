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
"Source/Core/Core.cpp"
"Source/Core/Application.cpp"
"Source/Core/Logging/Logger.cpp"
"Source/Core/Logging/Console.cpp"
"Source/Core/Memory/StackAllocator.cpp"
"Source/Core/Memory/STLAllocator.cpp"
"Source/Core/Memory/FreeListAllocator.cpp"
"Source/Core/Memory/MemoryManager.cpp"
"Source/Core/LayerStack.cpp"
"Source/Core/Layer.cpp"
"Source/Core/Configuration/ConfigManager.cpp"

"Source/Display/Linux/LinuxWindow.cpp"

"Source/Input/Linux/LinuxInput.cpp"
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/QMBTPCH.hpp")

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
      _CRT_SECURE_NO_WARNINGS
      GLFW_INCLUDE_NONE
    PUBLIC
      $<$<CONFIG:Debug>:QMBT_DEBUG>
      $<$<CONFIG:RelWithDebInfo>:QMBT_DEBUG>
      $<$<CONFIG:Release>:QMBT_RELEASE>
      $<$<CONFIG:MinSizeRel>:QMBT_RELEASE>
)


target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/EntryPoint")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")



# =======================================================
# Appending to CMAKE_PREFIX_PATH for vendor libraries
# =======================================================


# =======================================================
# Searching for Vendor Packages
# =======================================================

# =======================================================
# Including vendor libraries
# =======================================================
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Vendor")



# =======================================================
# Adding libraries with CMakeLists.txt as subdirectories
# =======================================================
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/magic_enum")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/eastl")

# set(EASTL_ROOT_DIR "${CMAKE_SOURCE_DIR}/Vendor/eastl")

# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/include")
# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/test/packages/EAAssert/include")
# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/test/packages/EABase/include/Common")
# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/test/packages/EAMain/include")
# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/test/packages/EAStdC/include")
# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/test/packages/EATest/include")
# target_include_directories (${PROJECT_NAME} PRIVATE "${EASTL_ROOT_DIR}/test/packages/EAThread/include")

# set(EASTL_LIBRARY debug "${EASTL_ROOT_DIR}/build/Debug/EASTL.lib" optimized "${EASTL_ROOT_DIR}/build/Release/EASTL.lib")
# add_custom_target(NatVis SOURCES "${EASTL_ROOT_DIR}/doc/EASTL.natvis")

# =======================================================
# Linking vendor libraries
# =======================================================
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC magic_enum::magic_enum)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)
target_link_libraries(${PROJECT_NAME} PUBLIC EASTL)






