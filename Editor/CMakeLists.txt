# ===================================================
# QOMBAT BUILD SYSTEM

# This is the CMakeLists.txt that generates the
# actual executable for the frontend application
# created using Qombat. The Engine is linked to it
# as a Static Library
# ===================================================

# Create the executable
add_executable(Editor "Source/Editor.cpp")

# Link the static library from Engine
# This will cause the include directories for that target to be added to this project
target_link_libraries(Editor PRIVATE Engine)