
add_library(Engine STATIC 
"Source/QMBT.hpp"
"Source/Core/Application.cpp"
"Source/Core/Log.cpp"
"Source/Core/Console.cpp"
)

set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)


target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/Vendor")


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog")

target_link_libraries(Engine PUBLIC spdlog::spdlog)