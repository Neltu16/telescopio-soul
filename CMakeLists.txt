cmake_minimum_required(VERSION 3.26)
project(telescopio_soul)

set(CMAKE_CXX_STANDARD 23)

include_directories(lib)
include_directories(${MSMPI_INC})
find_package(MPI REQUIRED)
add_executable(telescopio_soul
        lib/stb_image_write.h
        main.cpp)

target_link_libraries(telescopio_soul PUBLIC MPI::MPI_CXX)