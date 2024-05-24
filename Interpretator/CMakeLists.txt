cmake_minimum_required(VERSION 3.10)

project(Interpretor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    headers
)

add_executable(Interpretor
    sources/EDI.cpp
    sources/object.cpp
    sources/interpretator.cpp  # Corrected file name
    sources/parser.cpp
    sources/symtab.cpp
)
