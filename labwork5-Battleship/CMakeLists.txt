cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
project(labwork5-maksimbelov1)
add_executable(labwork5-maksimbelov1 main.cpp
            Game.cpp
            Game.h
            Strategy.cpp
            Strategy.h
            Field.cpp
            Field.h
         
)

enable_testing()
add_subdirectory(tests)