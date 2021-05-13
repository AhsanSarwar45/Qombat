# ===================================================
# QOMBAT BUILD SYSTEM

# This is the CMakeLists.txt that generates the
# actual executable for the frontend application
# created using Qombat. The Engine is linked to it
# as a Static Library
# ===================================================

project(Editor)

# Create the executable
add_executable(${PROJECT_NAME} 
"Source/Editor.cpp"
"Source/Panels/TestPanel.cpp"
"Source/Panels/ProfilerPanel.cpp"
"Source/ImGui/ImGuiLayer.cpp"
"Source/ImGui/OpenGL/ImGuiOpenGL.cpp"
"Source/ImGui/GLFW/ImGuiGLFW.cpp"
"Source/ImGui/Utility/Colors.cpp"

"Vendor/imgui/imgui.cpp"
"Vendor/imgui/imgui_draw.cpp"
"Vendor/imgui/imgui_demo.cpp"
"Vendor/imgui/imgui_tables.cpp"
"Vendor/imgui/imgui_widgets.cpp"

"Vendor/implot/implot.cpp"
"Vendor/implot/implot_items.cpp"
"Vendor/implot/implot_demo.cpp"

)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")


target_compile_definitions(${PROJECT_NAME}
    PRIVATE
      _CRT_SECURE_NO_WARNINGS
      GLFW_INCLUDE_NONE
)


# =======================================================
# Including vendor libraries
# =======================================================
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vendor")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui")


# Link the static library from Engine
# This will cause the include directories for that target to be added to this project
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)