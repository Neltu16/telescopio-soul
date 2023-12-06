cmake_minimum_required(VERSION 3.26)
project(telescopio_soul)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)

add_executable(telescopio_soul
        lib/stb_image_write.h
        main.cpp)
