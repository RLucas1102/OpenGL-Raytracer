cmake_minimum_required(VERSION 3.26.0)
project(depthTesting VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(depthTesting
    src/main.cpp
    src/glad.c
)

add_custom_command(
    TARGET depthTesting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory              
    ${CMAKE_SOURCE_DIR}/files $<TARGET_FILE_DIR:depthTesting>/files
    COMMENT "Copying shaders" VERBATIM
)

target_include_directories(depthTesting PRIVATE "../include")

target_link_libraries(depthTesting ${GLFW3_LIBRARY} OpenGL::GL)
