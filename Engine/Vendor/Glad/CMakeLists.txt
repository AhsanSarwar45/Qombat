cmake_minimum_required(VERSION 3.16)
project(glad)

add_library(${PROJECT_NAME} include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)