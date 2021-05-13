# ===================================================
# QOMBAT BUILD SYSTEM

# This is the CMakeLists.txt that generates the
# executable for the Tests
# ===================================================

project(Tests)

add_subdirectory("Vendor/Catch2")

add_executable(${PROJECT_NAME} 
"Source/Main.cpp"
"Source/ConfigurationTest.cpp"
"Source/StackAllocatorTest.cpp"
"Source/PoolAllocatorTest.cpp"

)

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Catch2/extras")

include(CTest)
include(Catch)

catch_discover_tests(${PROJECT_NAME})