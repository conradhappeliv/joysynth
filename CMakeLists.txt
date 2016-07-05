cmake_minimum_required(VERSION 3.2)
project(joysynth2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb")

set(SOURCE_FILES main.cpp)
add_executable(joysynth2 ${SOURCE_FILES}
    Synthesizer.cpp
    Synthesizer.h
    Controller.cpp
    Controller.h
    Plotter.cpp
    Plotter.h
    Sine.cpp
    Sine.h
    Square.cpp
    Square.h Audio.cpp Audio.h Effect.cpp Effect.h ConvReverb.cpp ConvReverb.h)
target_link_libraries(joysynth2
                        "jack"
                        "pthread"
                        "fftw3"
                        "boost_iostreams"
                        "boost_system"
                        "boost_filesystem")