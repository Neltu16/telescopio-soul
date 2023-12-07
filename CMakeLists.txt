cmake_minimum_required(VERSION 3.23)
project(telescopio_soul)

find_package(MPI REQUIRED)
set(MPI_CXX_LIBRARIES ${MPI_CXX_LIBRARIES})

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_C_COMPILER mpicc)

set(CMAKE_CXX_STANDARD 14)

add_executable(telescopio_soul main.cpp)

target_link_libraries(telescopio_soul ${MPI_CXX_LIBRARIES})