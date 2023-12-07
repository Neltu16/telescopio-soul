# CMakeLists.txt en la carpeta principal del proyecto (telescopio-soul)

cmake_minimum_required(VERSION 3.22)
project(telescopio_soul)

# Buscar MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# Configurar la ubicación de los archivos fuente y de encabezado
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(DIST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dist)

# Configurar la ubicación de stb_image.h y stb_image_write.h
include_directories(${LIB_DIR})

# Configurar el compilador MPI
set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_C_COMPILER mpicc)

# Configurar el estándar de C++
set(CMAKE_CXX_STANDARD 14)

# Agregar el ejecutable en la carpeta dist
set(EXECUTABLE_OUTPUT_PATH ${DIST_DIR})
add_executable(telescopio_soul ${SOURCE_DIR}/main.cpp)

# Vincular las bibliotecas MPI
target_link_libraries(telescopio_soul ${MPI_CXX_LIBRARIES})