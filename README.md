# telescopio-soul

Este programa combina los canales de color de diferentes archivos de texto para crear una imagen en formato PNG. Los archivos de entrada deben contener valores de píxeles para los canales alfa, rojo, verde, azul y promedio.

## Uso

./nombre_del_programa alfa.txt rojo.txt verde.txt azul.txt promedio.txt (temporal)

El programa sigue los siguientes pasos:

Abre los archivos de entrada directamente con los nombres proporcionados en la línea de comandos.
Lee los valores de píxeles de los canales alfa, rojo, verde, azul y promedio y los almacena en un solo arreglo 
Combina los valores de píxeles de los canales para cada posición y los convierte a un formato adecuado.
Crea una imagen en formato PNG con los valores combinados.

Requisitos
stb_image_write.h: Biblioteca para escribir imágenes en formato PNG. 

Notas
Los archivos de entrada deben contener valores válidos para los píxeles de los canales. El programa maneja el caso especial donde un canal tiene el valor "*", utilizando el valor promedio en su lugar. (temporal)


