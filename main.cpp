#include <iostream>
#include <fstream>
#include <chrono>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

const int width = 10681;
const int height = 7121;
const int channels = 4;  // (R, G, B, A)

void closeFiles(std::ifstream& alfaFile, std::ifstream& rojoFile, std::ifstream& verdeFile, std::ifstream& azulFile, std::ifstream& promedioFile) {
    alfaFile.close();
    rojoFile.close();
    verdeFile.close();
    azulFile.close();
    promedioFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Uso: " << argv[0] << " alfa.txt rojo.txt verde.txt azul.txt promedio.txt\n";
        return 1;
    }

    // Obtener el tiempo de inicio
    auto start_time = std::chrono::high_resolution_clock::now();

    // Abrir los archivos directamente con los nombres obtenidos de argv
    std::ifstream alfaFile(argv[1]), rojoFile(argv[2]), verdeFile(argv[3]), azulFile(argv[4]), promedioFile(argv[5]);

    if (!(alfaFile && rojoFile && verdeFile && azulFile && promedioFile)) {
        std::cerr << "Error al abrir uno de los archivos\n";
        return 1;
    }

    // Crear un solo arreglo para almacenar los píxeles de todos los canales, movimiento a través de files, columnas y su color
    auto* imagePixels = new unsigned char[width * height * channels];

    // Leer los valores de los archivos y almacenarlos directamente en el arreglo
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::string alfaValue, rojoValue, verdeValue, azulValue, promedioValue;

            alfaFile >> alfaValue;
            rojoFile >> rojoValue;
            verdeFile >> verdeValue;
            azulFile >> azulValue;
            promedioFile >> promedioValue;

            // Convertir a entero y almacenar directamente en el arreglo unidimensional (R,G,B,A) y se repite...
            int index = (i * width + j) * channels;

            // Modificar según tus condiciones
            imagePixels[index]     = (rojoValue == "*") ? static_cast<unsigned char>(std::stoi(promedioValue) * 0.3) : static_cast<unsigned char>(std::stoi(rojoValue));
            imagePixels[index + 1] = (verdeValue == "*") ? static_cast<unsigned char>(std::stoi(promedioValue) * 0.59) : static_cast<unsigned char>(std::stoi(verdeValue));
            imagePixels[index + 2] = (azulValue == "*") ? static_cast<unsigned char>(std::stoi(promedioValue) * 0.11) : static_cast<unsigned char>(std::stoi(azulValue));
            imagePixels[index + 3] = static_cast<unsigned char>(std::stoi(alfaValue));
        }
    }

    // Cerrar los archivos
    closeFiles(alfaFile, rojoFile, verdeFile, azulFile, promedioFile);

    // Escribir la imagen en formato PNG
    stbi_write_png("combinada_image.png", width, height, channels, imagePixels, width * channels);

    // Obtener el tiempo de finalización
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Tiempo transcurrido: " << duration.count() << " milisegundos\n";

    // Liberar la memoria
    delete[] imagePixels;

    return 0;
}



