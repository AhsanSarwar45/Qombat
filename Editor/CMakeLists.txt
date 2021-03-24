# Create the executable
add_executable(Editor "src/Editor.cpp")

# Link the static library from Engine
# This will cause the include directories for that target to be added to this project
target_link_libraries(Editor PRIVATE Engine)