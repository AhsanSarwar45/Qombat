# ===================================================
# QOMBAT BUILD SYSTEM

# This is the CMakeLists.txt that generates the
# actual executable for the frontend application
# created using Qombat. The Engine is linked to it
# as a Static Library
# ===================================================

project(Editor)

# Create the executable
add_executable(${PROJECT_NAME} "Source/Editor.cpp")

# Link the static library from Engine
# This will cause the include directories for that target to be added to this project
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)