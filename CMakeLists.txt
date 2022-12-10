cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(TelnetDebugger LANGUAGES C)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)
include_directories(${CMAKE_BINARY_DIR}/src)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")