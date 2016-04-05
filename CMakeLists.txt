cmake_minimum_required(VERSION 3.2)
project(joysynth2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(joysynth2 ${SOURCE_FILES}
    Synthesizer.cpp
    Synthesizer.h
    Controller.cpp
    Controller.h)
target_link_libraries(joysynth2
                        "jack"
                        "pthread")