# telescopio-soul

Este programa combina los canales de color de diferentes archivos de texto para crear una imagen en formato PNG. Los archivos de entrada deben contener valores de píxeles para los canales alfa, rojo, verde, azul y promedio.

## Requisitos 
OpenMPI 
Cmake al menos 3.22 para su compilación.
stb_image_write.h Biblioteca para escribir imágenes en formato PNG
stb_image.h       Biblioteca para manipular imagenes

## Uso

1. Clonar este repositorio
2. Acceder al directorio del projecto y ejecutar 'cmake .' y luego 'make'
3. El binario debería encontrarse en la carpeta dist acceder al directorio y arrastar el hostfile correspondiente a src y los archivos .txt a dist
5. Debería recibir los argumentos con mpirun hostfile maquinas.txt ./telescopio_soul alfa.txt rojo.txt verde.txt azul.txt promedio.txt El programa sigue los siguientes pasos:

El programa realiza lo siguiente: 
Abre los archivos de entrada directamente con los nombres proporcionados en la línea de comandos. 
Lee los valores de píxeles de los canales alfa, rojo, verde, azul y promedio y los almacena en un solo arreglo unidimensional, esto es debido a que la función que escribe la imagen lee de esa forma los canales. 
Combina los valores de píxeles de los canales para cada posición y los convierte a un formato adecuado, en este caso se guardan cadenas y se convierten a enteros para el calculo de valores perdidos.
Dependiendo de la cantidad de procesos guarda "Imagenes parciales" pero no se generan, para después crear una imagen en formato PNG con los valores combinados.
 
Nota: Los archivos de entrada deben contener valores válidos para los píxeles de los canales. El programa maneja el caso especial donde un canal puede tener el valor "*", utilizando el valor promedio para calcularlo en su lugar.

Instalación de OpenMPI
Asegúrate de tener acceso a permisos de administrador.

Actualizar el Sistema

'sudo apt update' 

Instalar OpenMPI

'sudo apt install openmpi-bin libopenmpi-dev'








