# ebnfparser/CMakeLists.txt

cmake_minimum_required(VERSION 3.28)

project(ebnfparser)

add_subdirectory(submodules)

add_subdirectory(src)

enable_testing()
