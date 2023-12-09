# telescopio-soul

Este programa combina los canales de color de diferentes archivos de texto para crear una imagen en formato PNG. Los archivos de entrada deben contener valores de píxeles para los canales alfa, rojo, verde, azul y promedio.  
El proyecto fue realizado para la asignatura de Computación Paralela 2023 segundo semestre.

## Requisitos 
- OpenMPI  
- Cmake al menos 3.22 para su compilación.  
- stb_image_write.h Biblioteca para escribir imágenes en formato PNG  
- stb_image.h       Biblioteca para manipular imagenes  (Aunque está incluido en el repositorio ya no se utilizó, es preferible no generar imagenes parciales debido al tamaño que tienen, puede ser omitido al compilar)

Asegúrate de tener acceso a permisos de administrador.

Actualizar el Sistema

```sudo apt update```

Instalar OpenMPI

```sudo apt install openmpi-bin libopenmpi-dev```  

## Uso

1. Clonar este repositorio
2. Acceder al directorio del projecto y ejecutar ```cmake .``` y luego ```make```
3. El binario debería encontrarse en la carpeta **dist/telescopio_soul.exe** acceder al directorio y arrastar el hostfile correspondiente en conjunto de los archivos .txt
5. Debería recibir los argumentos con ```mpirun -hostfile maquinas.txt ./telescopio_soul alfa.txt rojo.txt verde.txt azul.txt promedio.txt```
6. Debido al tamaño de los archivos y que no se especifica en las instrucciones debería haber una copia de los archivos .txt en una ruta similar en cada nodo en conjunto a su binario y los archivos .txt y hostfile. Otra alternativa es clonar el repositorio a una carpeta compartida que todos los nodos tengan acceso desde una misma ruta relativa.

El programa realiza lo siguiente:  
1. Abre los archivos de texto con los argumentos proporcionados  
2. Distribuye la cantidad de filas que cada nodo va a leer
3. Lee los valores, sustituye los valores " * " según el promedio y su color y los almacena en vectores (bloques) que luego se recopilan en un vector para el proceso 0.
4. Por último se llama a la función que escribe la imagen desde el vector en formato [R][G][B][A][R][G]...
5. Los tiempos que imprime solo se relacionan con la recolección de valores de los canales y no toma en cuenta la generación de imagen final.
Nota: Los archivos de entrada deben contener valores válidos para los píxeles de los canales. El programa maneja el caso especial donde un canal puede tener el valor "*", utilizando el valor promedio para calcularlo en su lugar.

Se utilizó la formula promedio = R*0.3 + G*0.59 + B*0.11 en un inicio pero se descartó debido a que los resultados al generar imagenes  con los casos eran peores que utilizar simplemente el factor de aporte del color.

La implementación consistía en los siguientes casos: 
RVA == *
RV == *
R A == *
VA == *
Solo uno de los 3 es == * o ninguno

Este bloque de código añadía alrededor de 10-14 segundos en una máquina virtual ubuntu con dos núcleos asignados de manera local.
```
if (rojoValue == "*" && verdeValue == "*" && azulValue == "*") {
                    // Caso: RVA = "*"
                    imageBlocks[localIndex] = static_cast<unsigned char>(std::stoi(promedioValue))*0.3;
                    imageBlocks[localIndex + 1 ]= static_cast<unsigned char>(std::stoi(promedioValue))*0.59;
                    imageBlocks[localIndex + 2 ]= static_cast<unsigned char>(std::stoi(promedioValue)) * 0.11;
                } else if (rojoValue == "*" && verdeValue == "*" && azulValue != "*") {
                    // Caso: RV = "*"
                    imageBlocks[localIndex] = static_cast<unsigned char>(std::stoi(promedioValue))*0.3;
                    imageBlocks[localIndex + 1 ]= static_cast<unsigned char>(std::stoi(promedioValue))*0.59;
                    imageBlocks[localIndex + 2 ]= static_cast<unsigned char>(std::stoi(azulValue));
                } else if (rojoValue == "*" && verdeValue != "*" && azulValue == "*") {
                    // Caso: R A = "*"
                    imageBlocks[localIndex] = static_cast<unsigned char>(std::stoi(promedioValue))*0.3;
                    imageBlocks[localIndex + 1 ]= static_cast<unsigned char>(std::stoi(verdeValue));
                    imageBlocks[localIndex + 2 ]= static_cast<unsigned char>(std::stoi(promedioValue))*0.11;
                } else if(rojoValue != "*" && verdeValue == "*" && azulValue == "*"){
                    // Caso: VA = "*"
                    imageBlocks[localIndex] = static_cast<unsigned char>(std::stoi(rojoValue));
                    imageBlocks[localIndex + 1 ]= static_cast<unsigned char>(std::stoi(promedioValue))*0.59;
                    imageBlocks[localIndex + 2 ]= static_cast<unsigned char>(std::stoi(promedioValue))*0.11;
                } else {
                    // Caso: solo uno de los 3 es "*" o ninguno
                    imageBlocks[localIndex] = (rojoValue == "*")
                                              ? static_cast<unsigned char>(std::min(std::max(0.0, (static_cast<double>(std::stoi(promedioValue)) - static_cast<double>(std::stoi(verdeValue)) * 0.59 - static_cast<double>(std::stoi(azulValue)) * 0.11) * 0.3), 255.0))
                                              : static_cast<unsigned char>(std::stoi(rojoValue));
                    imageBlocks[localIndex + 1] = (verdeValue == "*")
                                                  ? static_cast<unsigned char>(std::min(std::max(0.0, (static_cast<double>(std::stoi(promedioValue)) - static_cast<double>(std::stoi(rojoValue)) * 0.3 - static_cast<double>(std::stoi(azulValue)) * 0.11) * 0.59), 255.0))
                                                  : static_cast<unsigned char>(std::stoi(verdeValue));
                    imageBlocks[localIndex + 2] = (azulValue == "*")
                                                  ? static_cast<unsigned char>(std::min(std::max(0.0, (static_cast<double>(std::stoi(promedioValue)) - static_cast<double>(std::stoi(rojoValue)) * 0.3 - static_cast<double>(std::stoi(verdeValue)) * 0.59) * 0.11), 255.0))
                                                  : static_cast<unsigned char>(std::stoi(azulValue));
                }
```




