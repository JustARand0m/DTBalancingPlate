cmake_minimum_required(VERSION 3.10)

project(RaspberryPi)

file(GLOB INCL src/*.h)

#gloabl header files / public header files
include_directories(include)
file(GLOB SOURCES src/*.cpp src/*.h)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(EXECUTABLE_OUTPUT_PATH ./bin)

add_executable("${PROJECT_NAME}" "${SOURCES}")

