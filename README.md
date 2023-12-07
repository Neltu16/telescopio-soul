# telescopio-soul
Este programa combina los canales de color de diferentes archivos de texto para crear una imagen en formato PNG. Los archivos de entrada deben contener valores de píxeles para los canales alfa, rojo, verde, azul y promedio.

## Requisitos 
## Uso

hostfile archivo.txt ./telescopio_soul alfa.txt rojo.txt verde.txt azul.txt promedio.txt
El programa sigue los siguientes pasos:

Abre los archivos de entrada directamente con los nombres proporcionados en la línea de comandos.
Lee los valores de píxeles de los canales alfa, rojo, verde, azul y promedio y los almacena en un solo arreglo.
Combina los valores de píxeles de los canales para cada posición y los convierte a un formato adecuado.
Crea una imagen en formato PNG con los valores combinados.
Requisitos
stb_image_write.h: Biblioteca para escribir imágenes en formato PNG.
Notas
Los archivos de entrada deben contener valores válidos para los píxeles de los canales. El programa maneja el caso especial donde un canal tiene el valor "*", utilizando el valor promedio para calcularlo en su lugar. 

## Instalación de OpenMPI
Asegúrate de tener acceso a permisos de administrador.

1. Actualizar el Sistema

sudo apt update
2. Instalar OpenMPI

sudo apt install openmpi-bin libopenmpi-dev

Configuración del Proyecto Telescopio Soul

1. Clonar el Repositorio

git clone https://github.com/tu-usuario/telescopio-soul.git
cd telescopio-soul
2. Configurar CMakeLists.txt
Asegúrate de que tu archivo CMakeLists.txt contenga las siguientes líneas para detectar y enlazar correctamente con OpenMPI:


# Agregar estas líneas al CMakeLists.txt
find_package(MPI REQUIRED)
set(MPI_CXX_LIBRARIES ${MPI_CXX_LIBRARIES})
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_C_COMPILER mpicc)
target_link_libraries(telescopio_soul ${MPI_CXX_LIBRARIES})
set(MPI_CXX_LIBRARIES ${MPI_CXX_LIBRARIES})
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_C_COMPILER mpicc)
target_link_libraries(telescopio_soul ${MPI_CXX_LIBRARIES})
