
add_library(Engine STATIC 
"src/QMBT.hpp"
"src/Core/Application.cpp"
)

set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
