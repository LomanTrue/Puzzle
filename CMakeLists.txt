cmake_minimum_required(VERSION 3.15)
project(project_puzzle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML COMPONENTS system window graphics audio REQUIRED)

add_executable(project_puzzle main.cpp)
target_link_libraries(project_puzzle sfml-system sfml-window sfml-graphics sfml-audio)

add_subdirectory(lib)