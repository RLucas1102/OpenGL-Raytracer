cmake_minimum_required(VERSION 3.26.0)
project(cmake_test VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(cmake_test
    src/main.cpp
    src/glad.c
)

target_include_directories(cmake_test PRIVATE "D:/Projects/CSCI471/include")

target_link_libraries(cmake_test ${GLFW3_LIBRARY} OpenGL::GL)